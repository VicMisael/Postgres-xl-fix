/*-------------------------------------------------------------------------
 *
 * pathnode.h
 *	  prototypes for pathnode.c, relnode.c.
 *
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Portions Copyright (c) 2012-2014, TransLattice, Inc.
 * Portions Copyright (c) 1996-2015, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/optimizer/pathnode.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef PATHNODE_H
#define PATHNODE_H

#include "nodes/relation.h"


/*
 * prototypes for pathnode.c
 */
extern int compare_path_costs(Path *path1, Path *path2,
				   CostSelector criterion);
extern int compare_fractional_path_costs(Path *path1, Path *path2,
							  double fraction);
extern void set_cheapest(RelOptInfo *parent_rel);
extern void add_path(RelOptInfo *parent_rel, Path *new_path);
extern bool add_path_precheck(RelOptInfo *parent_rel,
				  Cost startup_cost, Cost total_cost,
				  List *pathkeys, Relids required_outer);

extern Path *create_seqscan_path(PlannerInfo *root, RelOptInfo *rel,
					Relids required_outer);
extern Path *create_samplescan_path(PlannerInfo *root, RelOptInfo *rel,
					   Relids required_outer);
extern IndexPath *create_index_path(PlannerInfo *root,
				  IndexOptInfo *index,
				  List *indexclauses,
				  List *indexclausecols,
				  List *indexorderbys,
				  List *indexorderbycols,
				  List *pathkeys,
				  ScanDirection indexscandir,
				  bool indexonly,
				  Relids required_outer,
				  double loop_count);
extern BitmapHeapPath *create_bitmap_heap_path(PlannerInfo *root,
						RelOptInfo *rel,
						Path *bitmapqual,
						Relids required_outer,
						double loop_count);
extern BitmapAndPath *create_bitmap_and_path(PlannerInfo *root,
					   RelOptInfo *rel,
					   List *bitmapquals);
extern BitmapOrPath *create_bitmap_or_path(PlannerInfo *root,
					  RelOptInfo *rel,
					  List *bitmapquals);
extern TidPath *create_tidscan_path(PlannerInfo *root, RelOptInfo *rel,
					List *tidquals, Relids required_outer);
extern AppendPath *create_append_path(RelOptInfo *rel, List *subpaths,
				   Relids required_outer);
extern MergeAppendPath *create_merge_append_path(PlannerInfo *root,
						 RelOptInfo *rel,
						 List *subpaths,
						 List *pathkeys,
						 Relids required_outer);
extern ResultPath *create_result_path(List *quals);
extern MaterialPath *create_material_path(RelOptInfo *rel, Path *subpath);
extern UniquePath *create_unique_path(PlannerInfo *root, RelOptInfo *rel,
				   Path *subpath, SpecialJoinInfo *sjinfo);
#ifdef XCP
extern Path *create_subqueryscan_path(PlannerInfo *root, RelOptInfo *rel,
						 List *pathkeys, Relids required_outer,
						 Distribution *distribution);
#else
extern Path *create_subqueryscan_path(PlannerInfo *root, RelOptInfo *rel,
						 List *pathkeys, Relids required_outer);
#endif
extern Path *create_functionscan_path(PlannerInfo *root, RelOptInfo *rel,
						 List *pathkeys, Relids required_outer);
extern Path *create_valuesscan_path(PlannerInfo *root, RelOptInfo *rel,
					   Relids required_outer);
extern Path *create_ctescan_path(PlannerInfo *root, RelOptInfo *rel,
					Relids required_outer);
extern Path *create_worktablescan_path(PlannerInfo *root, RelOptInfo *rel,
						  Relids required_outer);
extern ForeignPath *create_foreignscan_path(PlannerInfo *root, RelOptInfo *rel,
						double rows, Cost startup_cost, Cost total_cost,
						List *pathkeys,
						Relids required_outer,
						List *fdw_private);
#ifdef PGXC
#ifndef XCP
extern Path *create_remotequery_path(PlannerInfo *root, RelOptInfo *rel);
#endif
#endif

extern Relids calc_nestloop_required_outer(Path *outer_path, Path *inner_path);
extern Relids calc_non_nestloop_required_outer(Path *outer_path, Path *inner_path);

extern NestPath *create_nestloop_path(PlannerInfo *root,
					 RelOptInfo *joinrel,
					 JoinType jointype,
					 JoinCostWorkspace *workspace,
					 SpecialJoinInfo *sjinfo,
					 SemiAntiJoinFactors *semifactors,
					 Path *outer_path,
					 Path *inner_path,
					 List *restrict_clauses,
					 List *pathkeys,
					 Relids required_outer);

extern MergePath *create_mergejoin_path(PlannerInfo *root,
					  RelOptInfo *joinrel,
					  JoinType jointype,
					  JoinCostWorkspace *workspace,
					  SpecialJoinInfo *sjinfo,
					  Path *outer_path,
					  Path *inner_path,
					  List *restrict_clauses,
					  List *pathkeys,
					  Relids required_outer,
					  List *mergeclauses,
					  List *outersortkeys,
					  List *innersortkeys);

extern HashPath *create_hashjoin_path(PlannerInfo *root,
					 RelOptInfo *joinrel,
					 JoinType jointype,
					 JoinCostWorkspace *workspace,
					 SpecialJoinInfo *sjinfo,
					 SemiAntiJoinFactors *semifactors,
					 Path *outer_path,
					 Path *inner_path,
					 List *restrict_clauses,
					 Relids required_outer,
					 List *hashclauses);

extern Path *reparameterize_path(PlannerInfo *root, Path *path,
					Relids required_outer,
					double loop_count);

/*
 * prototypes for relnode.c
 */
extern void setup_simple_rel_arrays(PlannerInfo *root);
extern RelOptInfo *build_simple_rel(PlannerInfo *root, int relid,
				 RelOptKind reloptkind);
extern RelOptInfo *find_base_rel(PlannerInfo *root, int relid);
extern RelOptInfo *find_join_rel(PlannerInfo *root, Relids relids);
extern RelOptInfo *build_join_rel(PlannerInfo *root,
			   Relids joinrelids,
			   RelOptInfo *outer_rel,
			   RelOptInfo *inner_rel,
			   SpecialJoinInfo *sjinfo,
			   List **restrictlist_ptr);
extern RelOptInfo *build_empty_join_rel(PlannerInfo *root);
extern AppendRelInfo *find_childrel_appendrelinfo(PlannerInfo *root,
							RelOptInfo *rel);
extern RelOptInfo *find_childrel_top_parent(PlannerInfo *root, RelOptInfo *rel);
extern Relids find_childrel_parents(PlannerInfo *root, RelOptInfo *rel);
extern ParamPathInfo *get_baserel_parampathinfo(PlannerInfo *root,
						  RelOptInfo *baserel,
						  Relids required_outer);
extern ParamPathInfo *get_joinrel_parampathinfo(PlannerInfo *root,
						  RelOptInfo *joinrel,
						  Path *outer_path,
						  Path *inner_path,
						  SpecialJoinInfo *sjinfo,
						  Relids required_outer,
						  List **restrict_clauses);
extern ParamPathInfo *get_appendrel_parampathinfo(RelOptInfo *appendrel,
							Relids required_outer);

#endif   /* PATHNODE_H */
