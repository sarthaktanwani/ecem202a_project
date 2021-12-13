/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
Using BLE module for NS3 from this paper:
@inproceedings{mishra:hal-03217312,
TITLE = {{SimBle: Comment g{'e}n{'e}rer des traces r{'e}elles Bluetooth conformes aux recommandations de pr{'e}servation de la vie priv{'e}e ?}},
AUTHOR = {Mishra, Abhishek K and Viana, Aline C and Achir, Nadjib},
URL = {https://hal.archives-ouvertes.fr/hal-03217312},
BOOKTITLE = {{ALGOTEL 2021 - 23{`e}mes Rencontres Francophones sur les Aspects Algorithmiques des T{'e}l{'e}communications}},
ADDRESS = {La Rochelle, France},
YEAR = {2021},
MONTH = Jun,
KEYWORDS = {Bluetooth ; randomisation d'adresses MAC ; NS-3 ; pr{'e}servation de la vie priv{'e}e},
PDF = {https://hal.archives-ouvertes.fr/hal-03217312/file/SimBle__Comment_g_n_rer_des_traces_r_elles_Bluetooth_conformes_aux_recommandations_de_pr_servation_de_la_vie_priv_e__.pdf},
HAL_ID = {hal-03217312},
HAL_VERSION = {v1},
GitLab URL: https://gitlab.inria.fr/mabhishe/simble/-/tree/master/
}
*/

#include "ns3/core-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ScratchSimulator");

int 
main (int argc, char *argv[])
{
  NS_LOG_UNCOND ("Scratch Simulator");
  CommandLine cmd;
  cmd.Parse (argc, argv);
}
