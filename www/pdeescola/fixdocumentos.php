<?
set_time_limit(30000);
$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";


// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$db = new cls_banco();

$sql = "SELECT count(*), docdsc FROM workflow.documento WHERE esdid in(34,32,31,33,35,36,39,40,876)
		GROUP BY docdsc 
		HAVING COUNT(*)>1";

$__dados = $db->carregar($sql);

if($__dados[0]) {
	$numeroproc=0;
	$numeroescrep=0;
	$numeroescnenhum=0;
	foreach($__dados as $d) {
		
		$sql = "SELECT * FROM workflow.documento WHERE docdsc='".$d['docdsc']."' AND tpdid='5' AND esdid='40' ORDER BY docid LIMIT 1";
		$documentos = $db->carregar($sql);
		
		$dsc = explode(" - ", $d['docdsc']);
		$sql = "SELECT * FROM pdeescola.memaiseducacao me 
				LEFT JOIN entidade.entidade en ON en.entid=me.entid 
				WHERE entnome='".$dsc[1]."' AND memanoreferencia='2009'";
		
		$planos = $db->carregar($sql);
		
		if(count($planos) > 1) {
			echo "Possivel escola repetida:".$dsc[1]."<br>";
			$numeroescrep++;
		} elseif($planos[0]) {
			foreach($planos as $pl) {
				if($documentos[0]) {
					foreach($documentos as $doc) {
						$_NOT[] = $pl['memid'];
						$sql = "UPDATE pdeescola.memaiseducacao SET docid='".$doc['docid']."' WHERE memid='".$pl['memid']."'";
						$db->executar($sql);
						echo $sql."ESC.".$dsc[1]."<br>";
						$numeroproc++;
					}
				}
			}
		} else {
			echo "Nenhum plano não encontrado:".$dsc[1]."<br>";
			$numeroescnenhum++;
		}
		
	}
	
	foreach($__dados as $d) {
		$sql = "SELECT * FROM workflow.documento WHERE docdsc='".$d['docdsc']."' AND tpdid='5' AND esdid='39' ORDER BY docid LIMIT 1";
		$documentos = $db->carregar($sql);
		
		$dsc = explode(" - ", $d['docdsc']);
		$sql = "SELECT * FROM pdeescola.memaiseducacao me 
				LEFT JOIN entidade.entidade en ON en.entid=me.entid 
				WHERE entnome='".$dsc[1]."' AND memanoreferencia='2009' ".(($_NOT)?"AND memid NOT IN('".implode("','", $_NOT)."')":"")."";
		
		$planos = $db->carregar($sql);
		
		if(count($planos) > 1) {
			echo "Possivel escola repetida:".$dsc[1]."<br>";
			$numeroescrep++;
		} elseif($planos[0]) {
			foreach($planos as $pl) {
				if($documentos[0]) {
					foreach($documentos as $doc) {
						$sql = "UPDATE pdeescola.memaiseducacao SET docid='".$doc['docid']."' WHERE memid='".$pl['memid']."'";
						$db->executar($sql);
						echo $sql."ESC.".$dsc[1]."<br>";
						$numeroproc++;
					}
				}
			}
		} else {
			echo "Nenhum plano não encontrado:".$dsc[1]."<br>";
			$numeroescnenhum++;
		}
	}
	
	
	echo "Foram processadas ".$numeroproc." Escolas<br>";
	echo "Foram encontradas possiveis ".$numeroescrep." Escolas repetidas<br>";
	echo "Foram encontradas ".$numeroescnenhum." Escolas sem plano em 2009<br>";
}

$db->commit();


?>