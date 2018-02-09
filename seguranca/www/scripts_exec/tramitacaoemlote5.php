<?php

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
//error_reporting(-1);
session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '80541607049';
$_SESSION['usucpf'] = '80541607049';

$db = new cls_banco();

$sql = "select docid from pdeescola.memaiseducacao where entcodent in ('12009407', '12001333', '12029645', '12007722', '27018601', '27018369', '27226913', '27021009', '13045717', '13049739', '13018205', '13051172', '13012479', '13257226', '13029720', '13028499', '13081527', '13076450', '13030078', '13029703', '13030507', '13093541', '13029576', '13029339', '13093177', '13030230', '13030736', '13092332', '13089242', '13030884', '13027255', '13258214', '13092855', '13040871', '13040863', '13074334', '13042009', '13059440', '13007300', '13007530', '13069640', '13013270', '13044656', '13037463', '16008162', '16008634', '29149916', '29256356', '29256933', '29257085', '29163480', '29205395', '29297389', '29166292', '29324114', '29355753', '29073103', '29073081', '29040078', '29169542', '29121302', '29453402', '29003385', '29055946', '29161517', '23207345', '23059265', '23179074', '23162899', '23229969', '23018682', '23035927', '23035625', '23035978', '23037369', '23037334', '23220813', '23037474', '23253746', '23238364', '23080256', '23080957', '23130342', '23190833', '23091886', '23171774', '23249714', '23043938', '23148772', '32001606', '32052260', '32034563', '32034814', '32034741', '32034830', '32043554', '32043694', '32021194', '32021259', '32021925', '32022190', '32021950', '32004532', '32059850', '32057032', '32014775', '32007175', '32017880', '32018088', '32036116', '32036531', '32077734', '32037180', '32039816', '32071493', '52070450', '52088790', '52070271', '52090620', '52092399', '52031551', '52070204', '52037150', '52076253', '52068773', '52036448', '52037258', '52036537', '52035964', '52036359', '52035891', '52035980', '52037240', '21100691', '21132500', '21254486', '21132526', '21133069', '21134022', '21222827', '21137897', '21067279', '21217289', '21153493', '21127840', '21128812', '21127832', '21492476', '21229767', '21130833', '21008728', '21016674', '21021562', '21164886', '31333247', '31315800', '50023179', '50023160', '50019287', '15065430', '15036626', '15040496', '15039900', '15042820', '15038769', '15042669', '15056694', '15536009', '15586375', '15575543', '15044181', '15133710', '25033204', '25023446', '25062638', '25062670', '25018094', '25059181', '25064134', '25064126', '25061402', '25083490', '25052497', '25090909', '25091000', '25000349', '25006223', '25091565', '25091689', '25091603', '25091638', '25007092', '25122908', '25067460', '25007904', '25007742', '25007718', '25117858', '25072021', '25072200', '25072005', '25072285', '25072374', '25076582', '25072234', '25072080', '25120670', '25072013', '25071394', '25072226', '25117866', '25072145', '25073826', '25114433', '25072218', '25114808', '25072161', '25070720', '25072242', '25001906', '25028022', '25027999', '25013866', '25115618', '25092111', '25044788', '25067869', '25061739', '25068636', '25080407', '25034219', '25080881', '25080849', '25081454', '25081462', '25081438', '25030655', '25030620', '25030698', '25124277', '25094734', '25094556', '25094181', '25094106', '25093967', '25094360', '25094050', '25093860', '25094173', '25094335', '25094327', '25094122', '25094840', '25093975', '25094084', '25094459', '25094726', '25093932', '25094629', '25094653', '25093622', '25094661', '25094750', '25114425', '25093835', '25097091', '25093940', '25093746', '25093843', '25094793', '25094041', '25095013', '25094114', '25094203', '25094211', '25114905', '25114506', '25094777', '25094688', '25094262', '25094440', '25094530', '25094980', '25094432', '25115049', '25094025', '25041525', '25077759', '25086936', '25035444', '25089013', '25088971', '25019538', '25078143', '25115359', '25045555', '25084526', '25014960', '25042351', '25020960', '25020870', '25020986', '25020650', '25020927', '25020900', '25021427', '25103563', '25020978', '25042998', '25117521', '25026402', '25115723', '25089250', '25070088', '25101072', '25118560', '25015893', '25015885', '25036343', '25036335', '25078550', '25081349', '25082922', '25017080', '25099256', '25116762', '25099175', '25099230', '25099159', '25003852', '25047116', '25089242', '25089552', '25062271', '25062557', '25062220', '25089820', '25089781', '25089773', '25048848', '25070576', '25060376', '25060392', '25058797', '25058304', '25018140', '25018183', '25018582', '25019228', '25018124', '25018221', '25018078', '25018639', '25018264', '25050486', '25012959', '25084950', '26081644', '26032163', '26155737', '26041901', '26174731', '26109972', '26101122', '26169720', '26035731', '26147181', '26035154', '26126680', '26125528', '26126788', '26123649', '26155869', '26125480', '26167883', '26011093', '26104890', '26103168', '26095807', '22100407', '22068546', '22122737', '22100725', '22100474', '22121820', '22024972', '41354184', '41380622', '33036810', '33036799', '33009236', '33094322', '33009031', '33009449', '33009201', '33009260', '33009210', '33146837', '33050619', '33049173', '33049343', '33117381', '33143021', '33054053', '33059497', '33145857', '33059829', '33073058', '33078092', '33069816', '33065152', '33069093', '33076111', '33088489', '33066272', '33084076', '33080372', '33062439', '33084823', '33069409', '33074585', '33076898', '33080267', '33071225', '33069263', '33068348', '33082588', '33070113', '33070482', '33069824', '33074747', '33084670', '33080070', '33076901', '33072710', '33069182', '33062447', '33080585', '33070660', '33089159', '33091510', '33091501', '33018448', '24036307', '24002364', '24057738', '24057584', '24066427', '11005980', '11015683', '43212026', '43013619', '43016286', '43015905', '43026982', '43029884', '43038549', '43038506', '43038638', '43038891', '43098967', '43173713', '43123422', '43128513', '43167888', '42124530', '42079438', '42003806', '28005201', '28070402', '28022424', '28029275', '28031903', '28021754', '35243942', '35082478', '35057745', '35063125', '35081231', '35227821', '35130965', '35078347', '35069413', '35356499', '35055897', '35071183', '35205813', '35229106', '35076326', '35023280', '35148520', '17000203', '17004977', '17012112', '17012970', '17021944', '17030412', '17026512', '17026610', '17019290', '17035287', '17035830', '17046386', '17036704', '17025621', '17036747', '17010012') and memanoreferencia = 2012
"; 


$lista = $db->carregar($sql);
if($lista[0]) {
	foreach($lista as $l) {
		$docid = $l['docid'];
		$aedid = 215;
		$dados = array();
		$result = wf_alterarEstado( $docid, $aedid, $cmddsc = 'Tramitar em lote 13/08/12 a pedido de Carla Medeiros', $dados);
	
	}
}


echo "fim";
?>