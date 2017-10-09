<?php
checkAno();

function popupJustificativa(){
	
	global $db;
	
	extract($_GET);
	
	$sqlaluno = "SELECT DISTINCT
				cae.caecpf as cpf,
				cae.caenome as  nome,
				per.perperiodo as periodo					
			FROM projovemurbano.pagamentoestudante pge
			INNER JOIN projovemurbano.cadastroestudante cae  ON pge.caeid = cae.caeid
			INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid
			INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
			WHERE 
				pge.docid =  {$docid}";

	$dadosaluno = $db->pegaLinha($sqlaluno);
	
	$sqllog = "SELECT 
				logresponse as resposta
					
			FROM 
				projovemurbano.logsgb 
			WHERE 
				logdocid = {$docid}
			AND logerro = 't'";
	
	$resposta= $db->carregarColuna($sqllog);
	
// 	if(is_array($resposta)){
		
// 		foreach ($resposta as $erro){
			
// 			$xml = simplexml_load_string($erro);
			
// 			ver($xml,d);
// 			$xml->registerXPathNamespace('soap', 'http://schemas.xmlsoap.org/soap/envelope/');
// 			$xml->registerXPathNamespace('xsi', 'http://www.w3.org/2001/XMLSchema-instance');
// 			$xml->registerXPathNamespace('xsd', 'http://www.w3.org/2001/XMLSchema');
			
// 			$test = (string) $xml->Body->SearchResponse->SearchResult->SearchURL;
// 			var_export($test);
// 			// Filtra dados do Array
// 			$retorno = Array();
// 			foreach( $vals as $val ){
// 				if( $val['level'] == 4 && substr($val['tag'],0,4) == 'KEY_' ){
// 					$x = explode('_',$val['tag']);
// 					$key = $x[1];
// 				}
// 				if( $val['level'] == 5 ){
// 					$retorno[$key][strtolower($val['tag'])] = utf8_decode($val['value']);
// 				}
// 			}
// 		die;
// 		}
// 	}
?>
	<html>
	<head>
	<title><?php echo NOME_SISTEMA; ?></title>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/prototype.js"></script>
	<script type="text/javascript" src="../includes/entidades.js"></script>
	<script type="text/javascript" src="/includes/estouvivo.js"></script>
	<script src="/emi/geral/js/emi.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
			<?php monta_titulo("Justificativa Rejeição","{$dadosaluno['cpf']}&nbsp&nbsp&nbsp{$dadosaluno['nome']}&nbsp&nbsp&nbspPeríodo:{$dadosaluno['periodo']}"); ?>
			<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
				<?foreach ($resposta as $erro){?>
					<tr>
						<td><?=$erro?></td>
					</tr>
				<?}?>
				<tr>
					<td colspan="2" style="text-align:center" class="SubtituloDireita"  ><input type="button" name="btn_fechar" value="Fechar" onclick="window.close()" /> </td>
				</tr>
			</table>
		</body>
	</html>
<?
}

function bloquearPagamento(){

	global $db;
	extract($_POST);
	
	 $cpf = explode(",", $caecpf);
	 $cpf = str_replace(array(".","-"),"",$cpf);
	
	if($caecpf !=''){
		foreach ($cpf as $aluno){
			if($aluno!='' && (strlen($aluno)== 11)){
				
				$sql = "UPDATE projovemurbano.cadastroestudante
						   SET caebloquearpagamento = 't'
						 WHERE
							caecpf  = '{$aluno}'";
				$db->executar($sql);
				$db->commit();
			}
		}
				echo "
				<script>
					alert('Bloqueio bem sucedido!');
					window.location.href = window.location.href;
				</script>
				";
				die();
	}else{
		echo "
		<script>
			alert('Insira ao menos um cpf');
			window.location.href = window.location.href;
		</script>
		";
		die();
	}
	
}

function desbloquearPagamento(){

	global $db;
	
	extract($_REQUEST);
	
	$cpf = str_pad($caecpf_desbloqueia, 11, "0", STR_PAD_LEFT);

	$sql = "UPDATE projovemurbano.cadastroestudante
			SET caebloquearpagamento = 'f'
			WHERE
				caecpf  = '{$cpf}'";
// 	ver($sql,d);
	$db->executar($sql);
	$db->commit();
	
	echo "
	<script>
		alert('Pagamento desbloqueado');
        window.location.href = window.location.href;	
	</script>
             ";
	die();
}

function monta_sql( $superuser ){
	
	global $db;
	
	if( $_POST['pjuesfera'] == 'E' ){
		if( $_POST['estuf'] != '' ){
			$where = " AND pju.estuf = '".$_POST['estuf']."'";
		}else{
			$where = "AND pju.estuf IS NOT NULL";
		}
	}
	if( $_POST['pjuesfera'] == 'M' ){
		if( $_POST['muncod'] != '' ){
			$where = "AND pju.muncod = '".$_POST['muncod']."'";
		}else{
			$where = "AND pju.muncod IS NOT NULL";
		}
	}
// 	ver($_GET,d);	
	// monta o sql
	
		$sql = "
				SELECT DISTINCT
					COALESCE('POLO '||cae.polid,'Não Possui') as polo,
					'NÚCLEO '||nuc.nucid as nucleo,
					tur.turdesc as turma,
					CASE WHEN log.logerro = 't'
					THEN cae.caecpf||'&nbsp|&nbsp'||cae.caenome ||'&nbsp-&nbsp'||esd.esddsc||'&nbsp&nbsp&nbsp<a class=justificativa docid=\"'||doc.docid||'\" style=\"cursor: pointer;\">Justificativa</a>'
					ELSE cae.caecpf||'&nbsp|&nbsp'||cae.caenome ||'&nbsp-&nbsp'||esd.esddsc
					END as cpf
				FROM projovemurbano.pagamentoestudante pge
				INNER JOIN workflow.documento doc                ON pge.docid = doc.docid
				INNER JOIN workflow.estadodocumento esd		 ON esd.esdid = doc.esdid
				INNER JOIN projovemurbano.logsgb log		 ON log.logdocid = doc.docid
				INNER JOIN projovemurbano.cadastroestudante cae  ON pge.caeid = cae.caeid
				INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid 
				INNER JOIN workflow.documento doc2               ON dia.docid = doc2.docid
				INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
				INNER JOIN projovemurbano.turma tur              ON dia.turid = tur.turid
				INNER JOIN projovemurbano.nucleo nuc             ON nuc.nucid = tur.nucid 
				INNER JOIN projovemurbano.projovemurbano pju     ON pju.pjuid = cae.pjuid
				LEFT JOIN projovemurbano.municipio muni 	 ON muni.munid = nuc.munid
				LEFT JOIN territorios.municipio mun 		 ON mun.muncod = pju.muncod
				WHERE 
					pge.pgeaptoreceber = 't' 
				AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0 
				AND per.perid = {$_POST['perid']}
				AND doc2.esdid IN(523,524)
				AND pju.estuf = 'AC'
				AND log.logerro = 't'
				ORDER BY 
					polo, 
					nucleo,
					turma
				";
// 	ver($sql,d);
	return $sql;
}

	function monta_agp(){
		
		$agp = array("agrupador" => array(),
					 "agrupadoColuna" => array( "documento",
												"qtd") );
									
		array_push($agp['agrupador'], array("campo" => "polo",
											"label" => "Polo") );
				
		array_push($agp['agrupador'], array("campo" => "nucleo",
											"label" => "Nucleo") );
		
		array_push($agp['agrupador'], array("campo" => "turma",
											"label" => "Turma") );
		array_push($agp['agrupador'], array("campo" => "cpf",
											"label" => "Aluno") );
		return $agp;
	}
	
	
function pesquisapagamento(){
	
	global $db;
	
	ini_set("memory_limit", "2048M");
	// set_time_limit(120);
	
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	$superuser = $db->testa_superuser();
	$agrup = monta_agp();
	$sql   = monta_sql($superuser);
// 	ver($sql,d);
	$dados = $db->carregar($sql);
	
	?>
	<html>
	    <head>
	        <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
	        <script type="text/javascript" src="../includes/funcoes.js"></script>
	        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	    </head>
	    <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
		<script type="text/javascript">
		$(document).ready(function(){
			$('.justificativa').click(function(){
				var docid = $(this).attr('docid');
				formulario.target = 'docid';
		        var janela = window.open('projovemurbano.php?modulo=principal/pagamento&acao=A&requisicao=popupJustificativa&docid='+docid, 'docid', 
				        				'width=800,height=400,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
		        janela.focus();
			});
		});
		</script>
		<form action="" method="get" name="formulario" id="docid">
		</form>
	<?php
	$r = new montaRelatorio();
	$r->setAgrupador($agrup, $dados); 
	$r->setColuna(null);
	$r->setTotNivel(true);
	$r->setMonstrarTolizadorNivel(false);
	$r->setTotalizador(false);
	$r->setBrasao(true);
	$r->setEspandir( $_REQUEST['expandir']);
	echo $r->getRelatorio();
	
	monta_sql($superuser);
	monta_agp();
	?>
	</body>
	</html>
<?
}
function carregaPendentesRejeitados(){
	
	global $db;
	
	extract($_POST);
	
	$sql = "
			SELECT DISTINCT 
				(			
				SELECT DISTINCT
					SUM(dia.diabolsaspendentes)
				FROM  projovemurbano.diario dia             			
				INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
				WHERE 
					dia.perid = per.perid
				) as pendentes,
					(
				SELECT DISTINCT
					SUM(dia.diabolsasrecusadas)
				FROM 
					projovemurbano.diario dia             		
				INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
				WHERE 
					dia.perid = per.perid
				)as rejeitadas	
			FROM 
				projovemurbano.diario dia 
			INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
			WHERE 
				per.perid = $perid
			";
                

	$dados = $db->pegaLinha($sql);
// 		ver($dados, d);
	echo simec_json_encode($dados);
    die;
}

ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once APPRAIZ . "includes/workflow.php";

function autorizaPagamentos(){

	global $db;
// ver($_REQUEST,$_POST,d);
	extract($_POST);
	
	
	
	$sqlchamadocumento = "
						SELECT DISTINCT
							doc.docid as docid,
							doc.esdid as esdid
						FROM projovemurbano.pagamentoestudante pge
						INNER JOIN workflow.documento doc                ON pge.docid = doc.docid 
						INNER JOIN projovemurbano.cadastroestudante cae        ON pge.caeid = cae.caeid
						INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid 
						INNER JOIN workflow.documento doc2               ON dia.docid = doc2.docid
						INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
						INNER JOIN projovemurbano.turma tur              ON dia.turid = tur.turid
						INNER JOIN projovemurbano.nucleo nuc             ON nuc.nucid = tur.nucid 
						INNER JOIN projovemurbano.nucleoagenciabancaria nab    ON tur.nucid = nab.nucid
						INNER JOIN projovemurbano.nucleoescola nue       ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
						INNER JOIN entidade.entidade entnuc              ON entnuc.entid = nue.entid 
						INNER JOIN entidade.endereco endnuc              ON endnuc.entid = entnuc.entid 
						INNER JOIN projovemurbano.projovemurbano pju     ON pju.pjuid = cae.pjuid
						LEFT JOIN projovemurbano.municipio muni 	 ON muni.munid = nuc.munid
						LEFT JOIN territorios.municipio mun 		 ON mun.muncod = pju.muncod
						WHERE 
							pge.pgeaptoreceber = 't' 
						AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0 
						AND doc.esdid IN (527,560)
						AND doc2.esdid IN(523,524)
						AND per.perid IN (".implode(', ',$perid).")
						";
	$documento = $db->carregar($sqlchamadocumento);
	if($documento[0]) {
		foreach($documento as $docid) {
			
			if($docid['esdid']== '527'){
				$dados = array(	"esdidorigem"		=> '527',
								"esdiddestino" 		=> '528');
			}elseif($docid['esdid']== '560'){
				$dados = array(	"esdidorigem"		=> '560',
								"esdiddestino" 		=> '528');
			}

			$sqlaedid = "
						SELECT 
							aedid
						FROM workflow.acaoestadodoc 
						WHERE 
							esdidorigem='".$dados['esdidorigem']."' 
						AND esdiddestino='".$dados['esdiddestino']."'";
			$aedid = $db->pegaUm($sqlaedid);
			
			$cmddsc = 'Autorizar Pagamento';
			$dados_wf = array();
// 			ver($docid['docid'],d);
			$result = wf_alterarEstado( $docid['docid'], $aedid, $cmddsc, $dados_wf);
// 			ver($result,$cmddsc,d);
			echo "<tr><td>".$docid['docid']."</td><td>".(($result)?"Tramitado sucesso":"Não tramitado")."</td></tr>";
			
		}
		
		$sqldiarios = "
					SELECT
						diaid
					FROM
						projovemurbano.diario d
					WHERE
						d.perid IN (".implode(', ',$perid).")";
		$diarios= $db->carregarColuna($sqldiarios);
		
		foreach($diarios as $diaid){
		
			$sqlcontadora = "
					SELECT DISTINCT
						(
						SELECT DISTINCT
							count(doc1.docid)
						FROM projovemurbano.pagamentoestudante pge
						INNER JOIN workflow.documento doc1               		ON pge.docid = doc1.docid
						INNER JOIN projovemurbano.cadastroestudante cae  		ON pge.caeid = cae.caeid
						INNER JOIN projovemurbano.diario dia             		ON pge.diaid = dia.diaid
						INNER JOIN workflow.documento doc3               		ON dia.docid = doc3.docid
						INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
						INNER JOIN projovemurbano.turma tur              		ON dia.turid = tur.turid
						INNER JOIN projovemurbano.nucleo nuc             		ON nuc.nucid = tur.nucid
						INNER JOIN projovemurbano.nucleoagenciabancaria nab     ON tur.nucid = nab.nucid
						INNER JOIN projovemurbano.nucleoescola nue       		ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
						INNER JOIN entidade.entidade entnuc              		ON entnuc.entid = nue.entid
						INNER JOIN entidade.endereco endnuc              		ON endnuc.entid = entnuc.entid
						INNER JOIN projovemurbano.projovemurbano pju     		ON pju.pjuid = cae.pjuid
						WHERE
							doc1.esdid = 527
						AND dia.diaid = $diaid
						AND doc3.esdid IN(523,524)
						AND pge.pgeaptoreceber = 't'
						AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
						)as pendentes,
						(
						SELECT DISTINCT
							count(doc1.docid)
						FROM projovemurbano.pagamentoestudante pge
						INNER JOIN workflow.documento doc1               		ON pge.docid = doc1.docid
						INNER JOIN projovemurbano.cadastroestudante cae  		ON pge.caeid = cae.caeid
						INNER JOIN projovemurbano.diario dia             		ON pge.diaid = dia.diaid
						INNER JOIN workflow.documento doc3               		ON dia.docid = doc3.docid
						INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
						INNER JOIN projovemurbano.turma tur              		ON dia.turid = tur.turid
						INNER JOIN projovemurbano.nucleo nuc             		ON nuc.nucid = tur.nucid
						INNER JOIN projovemurbano.nucleoagenciabancaria nab     ON tur.nucid = nab.nucid
						INNER JOIN projovemurbano.nucleoescola nue       		ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
						INNER JOIN entidade.entidade entnuc              		ON entnuc.entid = nue.entid
						INNER JOIN entidade.endereco endnuc              		ON endnuc.entid = entnuc.entid
						INNER JOIN projovemurbano.projovemurbano pju     		ON pju.pjuid = cae.pjuid
						WHERE
							doc1.esdid = 528
						AND dia.diaid = $diaid
						AND doc3.esdid IN(523,524)
						AND pge.pgeaptoreceber = 't'
						AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
						)as autorizadas,
						(
						SELECT DISTINCT
							count(doc1.docid)
						FROM projovemurbano.pagamentoestudante pge
						INNER JOIN workflow.documento doc1               		ON pge.docid = doc1.docid
						INNER JOIN projovemurbano.cadastroestudante cae  		ON pge.caeid = cae.caeid
						INNER JOIN projovemurbano.diario dia             		ON pge.diaid = dia.diaid
						INNER JOIN workflow.documento doc3               		ON dia.docid = doc3.docid
						INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
						INNER JOIN projovemurbano.turma tur              		ON dia.turid = tur.turid
						INNER JOIN projovemurbano.nucleo nuc             		ON nuc.nucid = tur.nucid
						INNER JOIN projovemurbano.nucleoagenciabancaria nab     ON tur.nucid = nab.nucid
						INNER JOIN projovemurbano.nucleoescola nue       		ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
						INNER JOIN entidade.entidade entnuc              		ON entnuc.entid = nue.entid
						INNER JOIN entidade.endereco endnuc              		ON endnuc.entid = entnuc.entid
						INNER JOIN projovemurbano.projovemurbano pju     		ON pju.pjuid = cae.pjuid
						WHERE
							doc1.esdid = 529
						AND dia.diaid = $diaid
						AND doc3.esdid IN(523,524)
						AND pge.pgeaptoreceber = 't'
						AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
						)as enviadas,
						(
						SELECT DISTINCT
							count(doc1.docid)
						FROM projovemurbano.pagamentoestudante pge
						INNER JOIN workflow.documento doc1               		ON pge.docid = doc1.docid
						INNER JOIN projovemurbano.cadastroestudante cae  		ON pge.caeid = cae.caeid
						INNER JOIN projovemurbano.diario dia             		ON pge.diaid = dia.diaid
						INNER JOIN workflow.documento doc3               		ON dia.docid = doc3.docid
						INNER JOIN projovemurbano.periodocurso per1      		ON per1.perid = dia.perid
						INNER JOIN projovemurbano.turma tur              		ON dia.turid = tur.turid
						INNER JOIN projovemurbano.nucleo nuc             		ON nuc.nucid = tur.nucid
						INNER JOIN projovemurbano.nucleoagenciabancaria nab     ON tur.nucid = nab.nucid
						INNER JOIN projovemurbano.nucleoescola nue       		ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
						INNER JOIN entidade.entidade entnuc              		ON entnuc.entid = nue.entid
						INNER JOIN entidade.endereco endnuc              		ON endnuc.entid = entnuc.entid
						INNER JOIN projovemurbano.projovemurbano pju     		ON pju.pjuid = cae.pjuid
						WHERE
							doc1.esdid = 560
						AND dia.diaid = $diaid
						AND doc3.esdid IN(523,524)
						AND pge.pgeaptoreceber = 't'
						AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
						)as rejeitadas
					FROM projovemurbano.pagamentoestudante pge
					INNER JOIN workflow.documento doc                ON pge.docid = doc.docid
					INNER JOIN projovemurbano.cadastroestudante cae        ON pge.caeid = cae.caeid
					INNER JOIN projovemurbano.diario dia             ON pge.diaid = dia.diaid
					INNER JOIN workflow.documento doc2               ON dia.docid = doc2.docid
					INNER JOIN projovemurbano.periodocurso per       ON per.perid = dia.perid
					INNER JOIN projovemurbano.turma tur              ON dia.turid = tur.turid
					INNER JOIN projovemurbano.nucleo nuc             ON nuc.nucid = tur.nucid
					INNER JOIN projovemurbano.nucleoagenciabancaria nab    ON tur.nucid = nab.nucid
					INNER JOIN projovemurbano.nucleoescola nue       ON nuc.nucid = nue.nucid AND nue.nuetipo='S' AND nue.nuestatus='A'
					INNER JOIN entidade.entidade entnuc              ON entnuc.entid = nue.entid
					INNER JOIN entidade.endereco endnuc              ON endnuc.entid = entnuc.entid
					INNER JOIN projovemurbano.projovemurbano pju     ON pju.pjuid = cae.pjuid
					WHERE
						pge.pgeaptoreceber = 't'
					AND (cae.caeqtddireitobolsa - cae.caeqtdbolsaprojovem) > 0
					AND dia.diaid = $diaid
					AND doc2.esdid IN(523,524)
					";
			
				$contados = $db->pegaLinha($sqlcontadora);
				if($contados){
					
				$sqlinsertqtds = "
								UPDATE projovemurbano.diario
								SET 
									diabolsaspendentes = {$contados['pendentes']}, 
									diabolsasautorizadas = {$contados['autorizadas']}, 
									diabolsasenviadas = {$contados['enviadas']}, 
									diabolsasrecusadas = {$contados['rejeitadas']}
								WHERE
									diaid =$diaid";
				// 		ver($sqlinsertqtds,d);
				$db->executar($sqlinsertqtds);
						$db->commit();
				}
			}
		
		echo "
			<script>
				alert('Tramitação bem sucedida.');
			</script>";
				
	} else {
		echo "
			<script>
				alert('Não foram encontrados registros.');
				window.location.href = window.location.href;
			</script>";
	}
	
}

if( $_REQUEST['requisicao'] ){
	$_REQUEST['requisicao']();
	die();
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

monta_titulo('Pagamento de Alunos','');

include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/datapicker.pt_Br.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" />
<script type="text/javascript" src="./js/projovemurbano.js"></script>
<?

$db->cria_aba($abacod_tela,$url,$parametros);
$aba = !$_GET['aba'] ? "painelpagamento" : $_GET['aba'];

$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/pagamento&acao=A&aba=$aba";

/*** Array com os itens da aba de identificação ***/							
$menu = array( 0 => array("id" => 1, "descricao" => "Painel de Pagamento", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/pagamento&acao=A&aba=painelpagamento")
			  ,1 => array("id" => 2, "descricao" => "Bloquear Pagamento", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/pagamento&acao=A&aba=bloquearpagamento")
			  ,2 => array("id" => 3, "descricao" => "Autorizar Pagamento", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/pagamento&acao=A&aba=autorizarpagamento")
			  ,3 => array("id" => 4, "descricao" => "Retorno FNDE", "link" => "/projovemurbano/projovemurbano.php?modulo=principal/pagamento&acao=A&aba=retornofnde")
			  );
echo "<br/>"; 
$perfis = pegaPerfilGeral();
if($db->testa_superuser()){

	echo montarAbasArray($menu, $abaAtiva);			  
	
	include_once "pagamento/".$aba.".inc";
}

 ?>
