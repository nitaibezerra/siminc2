<?php
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
require_once APPRAIZ . "includes/classes/AbaAjax.class.inc";
//header('Content-Type: text/html; charset=utf-8', true);
header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

$usucpf = $_SESSION['usucpf'];
$pflcod = arrayPerfil();
$where = '';

/*
if( $pflcod == 200 ){
	$sql = "SELECT estuf FROM quest.usuarioresponsabilidade WHERE usucpf = '{$usucpf}' AND rpustatus = 'A' AND pflcod = ".$pflcod;
	$estuf = $db->pegaUm( $sql );
	if( $estuf ){
		$where = "WHERE m.estuf IN ('".$estuf."')";
	}
}
*/

if(	$_REQUEST['carregaMunicipio'] && $_REQUEST['estuf'] ){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = "SELECT
				'<img style=\"cursor:pointer\" id=\"img_dimensao_' || m.muncod || '\" src=\"/imagens/mais.gif\" 
				onclick=\"carregaEscolasQuest(this.id,\'' || m.muncod || '\');\" 
				border=\"0\" /><a href=\"#\" onclick=\"responderQuestM(\''||m.muncod||'\');\" title=\"Responder Questionário por Município\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
				' as acao,
				m.muncod as codigo,
				m.mundescricao ||
				'</td></tr>
			       	<tr style=\"display:none\" id=\"listaEscolasQuest_' || m.muncod || '\" >
			    	<td id=\"trV_' || m.muncod || '\" colspan=8 ></td>
			    </tr>' as linha
			FROM
				territorios.municipio m
			WHERE
				m.estuf = '".$_REQUEST['estuf']."'
			ORDER BY
				linha";

	$cabecalho = array("Ação","Código","Município");
	//$alinha = array("center","center","left");
	//$tamanho = array("10%","10%","80%");
	$db->monta_lista( $sql, $cabecalho, 100000, 10, 'N','','');
	exit();
}

if(	$_REQUEST['carregaEscola'] && $_REQUEST['muncod'] ){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = "SELECT 
				'<a href=\"#\" onclick=\"responderQuestEscola(\''||ent.entid||'\');\" title=\"Responder Questionário por Município\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
				' || ent.entnome  
			FROM
				entidade.entidade ent				
			INNER JOIN
				entidade.endereco ende ON ende.entid = ent.entid
			INNER JOIN
				entidade.funcaoentidade fe ON fe.entid = ent.entid
			INNER JOIN
				territorios.municipio m ON m.muncod = ende.muncod
			WHERE
				ent.entstatus='A' AND 
				fe.funid = 3 AND
				(ent.tpcid = 1 or ent.tpcid = 3) AND
				m.muncod = '{$_REQUEST['muncod']}'	
			ORDER BY
				ent.entnome";

	$cabecalho = array("Nome da Escola");
	$db->monta_lista( $sql, $cabecalho, 100000, 10, 'N','','');
	exit();
}


$ano = $_SESSION['exercicio'] == '' ? $_SESSION['exercicio_atual'] : $_SESSION['exercicio'];

if( in_array(PDEESC_PERFIL_SUPER_USUARIO, $pflcod) ){
	$perfil = PDEESC_PERFIL_SUPER_USUARIO;
} elseif( in_array( PDEESC_PERFIL_SEC_ESTADUAL_QUEST_SEESP, $pflcod) ){
	$perfil = PDEESC_PERFIL_SEC_ESTADUAL_QUEST_SEESP;
} elseif( in_array( PDEESC_PERFIL_SEC_MUNICIPAL_QUEST_SEESP, $pflcod) ){
	$perfil = PDEESC_PERFIL_SEC_MUNICIPAL_QUEST_SEESP;
} elseif( in_array( PDEESC_PERFIL_ESCOLA_QUEST_SEESP, $pflcod) ){
	$perfil = PDEESC_PERFIL_ESCOLA_QUEST_SEESP;
} elseif( in_array( PDEESC_PERFIL_ADM_QUEST_SEESP, $pflcod) ){
	$perfil = PDEESC_PERFIL_ADM_QUEST_SEESP;
} elseif( in_array( PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL, $pflcod) ){
	$perfil = PDEESC_PERFIL_CAD_ESCOLA_ACESSIVEL;
} else {
	$perfil = $pflcod[0];	
}

/*
if (!$_SESSION['entid']){
	die('<script>
			history.go(-1);
		 </script>');
}

$entid = $_SESSION['entid'];
$_SESSION['pdeid'] = pegarPdeid($entid);
$pdeid = $_SESSION['pdeid'];
*/




include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
//echo montarAbasArray(carregaAbas(), "/pdeescola/pdeescola.php?modulo=questionario&acao=A");

$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo='Painel';
monta_titulo($titulo_modulo,'Questionário SEESP<br>Respondido<img src="/imagens/check_p.gif" style="border:0;">
							 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
						     Não Respondido<img src="/imagens/atencao.png" style="border:0;">');

//echo cabecalhoPDE();
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
function carregaMunicipiosQuest(idImg, estuf){
	var img 	 = document.getElementById( idImg );
	var tr_nome = 'listaMunicipiosQuest_'+ estuf;
	var td_nome  = 'trV_'+ estuf;
	
	if(document.getElementById(tr_nome).style.display == 'none' && document.getElementById(td_nome).innerHTML == ""){
		document.getElementById(td_nome).innerHTML = 'Carregando...';
		img.src = '../imagens/menos.gif';
		carregaMunicipios(estuf, td_nome);
	}if(document.getElementById(tr_nome).style.display == 'none' && document.getElementById(td_nome).innerHTML != ""){
		document.getElementById(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
	} else {
		document.getElementById(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}

function carregaEscolasQuest(idImg2, muncod){
	var img2 	 = document.getElementById( idImg2 );
	var tr_nome2 = 'listaEscolasQuest_'+ muncod;
	var td_nome2  = 'trV_'+ muncod;

	if(document.getElementById(tr_nome2).style.display == 'none' && document.getElementById(td_nome2).innerHTML == ""){
		document.getElementById(td_nome2).innerHTML = 'Carregando...';
		img2.src = '../imagens/menos.gif';
		carregaEscolas(muncod, td_nome2);
	}if(document.getElementById(tr_nome2).style.display == 'none' && document.getElementById(td_nome2).innerHTML != ""){
		document.getElementById(tr_nome2).style.display = '';
		img2.src = '../imagens/menos.gif';
	} else {
		document.getElementById(tr_nome2).style.display = 'none';
		img2.src = '/imagens/mais.gif';
	}
}

function carregaMunicipios(estuf, td_nome){
	var myajax = new Ajax.Request('pdeescola.php?modulo=questionario&acao=D', {
		        method:     'post',
		        parameters: '&carregaMunicipio=true&estuf='+estuf,
		        asynchronous: false,
		        onComplete: function (res){
					document.getElementById(td_nome).innerHTML = res.responseText;
		        }
		  });
}

function carregaEscolas(muncod, td_nome2){
	var myajax = new Ajax.Request('pdeescola.php?modulo=questionario&acao=D', {
		        method:     'post',
		        parameters: '&carregaEscola=true&muncod='+muncod,
		        asynchronous: false,
		        onComplete: function (res){
					document.getElementById(td_nome2).innerHTML = res.responseText;
		        }
		  });
}

function responderQuest( estuf ) {
	window.location.href = "pdeescola.php?modulo=questionario&acao=C&aba=4&estuf="+estuf;
}

function responderQuestM( muncod ) {
	window.location.href = "pdeescola.php?modulo=questionario&acao=B&aba=2&muncod="+muncod;
}

function responderQuestEscola( entid ) {
	window.location.href = "pdeescola.php?modulo=questionario&acao=A&aba=1&entid="+entid;
}

</script>
<center>
<fieldset style="width: 94%; background: #fff;"  >
<legend>Questionários</legend>
<?php 

if( $_GET['acao'] == 'D' ){
	
	$sql = "SELECT DISTINCT
			'<img style=\"cursor:pointer\" id=\"img_dimensao_' || m.estuf || '\" src=\"/imagens/mais.gif\" 
			onclick=\"carregaMunicipiosQuest(this.id,\'' || m.estuf || '\');\" 
			border=\"0\" /> 
			<a href=\"#\" onclick=\"responderQuest(\''||m.estuf||'\');\" title=\"Responder Questionário\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'
			|| m.estuf ||
			'</td></tr>
		            	<tr style=\"display:none\" id=\"listaMunicipiosQuest_' || m.estuf || '\" >
		            		<td id=\"trV_' || m.estuf || '\" colspan=8 ></td>
		            </tr>' as linha
		FROM
			territorios.estado m
		{$where}
		ORDER BY
			linha";

	$db->monta_lista( $sql, array( "UF" ), 30, 10, 'N', '', '');
	
} else {
	$val = '';
	$val.= $_REQUEST['estuf'] ? '&estuf='.$_REQUEST['estuf'] : '';
	$val.= $_REQUEST['muncod'] ? '&muncod='.$_REQUEST['muncod'] : '';
	$val.= $_REQUEST['entid'] ? '&entid='.$_REQUEST['entid'] : '';
	
	cabecalhoQuestionario( $_GET['acao'], $perfil );
	
	if($_GET['acao'] == 'A'){
		
		$qrpid = pegaQrpid( $_GET['acao'], $_GET['aba'], $perfil, $ano );
		if( $_REQUEST['entid'] ){
			$menu = array(
						0 => array("id" => 0, "descricao" => "Programa de Implantação de Salas de Recursos Multifuncionais – Edição 2008", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=A&aba=1".$val),
						1 => array("id" => 1, "descricao" => "Programa Escola Acessível – Edição 2008", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=A&aba=2".$val)
						);
			echo montarAbasArray($menu, "/pdeescola/pdeescola.php?modulo=questionario&acao=A&aba=".$_GET['aba']."".$val);
		} else {
			$entid 	= questRecuperaResponsabilidadePerfil('entid');
			$questEsc = verificaQuestionario( $entid );
			if($questEsc['eeqsrm'] == 't' && $questEsc['eeqea'] == 't'){
				$qrpid = pegaQrpid( $_GET['acao'], $_GET['aba'], $perfil, $ano );
				$menu = array(
							0 => array("id" => 0, "descricao" => "Programa de Implantação de Salas de Recursos Multifuncionais – Edição 2008", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=A&aba=1"),
							1 => array("id" => 1, "descricao" => "Programa Escola Acessível – Edição 2008", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=A&aba=2")
							);
				echo montarAbasArray($menu, "/pdeescola/pdeescola.php?modulo=questionario&acao=A&aba=".$_GET['aba']);
			}elseif($questEsc['eeqsrm'] == 't' && $questEsc['eeqea'] == 'f'){
				$qrpid = pegaQrpid( $_GET['acao'], 1, $perfil, $ano );
				$menu = array(
							0 => array("id" => 0, "descricao" => "Programa de Implantação de Salas de Recursos Multifuncionais – Edição 2008", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=A")
							);
				echo montarAbasArray($menu, "/pdeescola/pdeescola.php?modulo=questionario&acao=A");
			}elseif($questEsc['eeqsrm'] == 'f' && $questEsc['eeqea'] == 't'){
				$qrpid = pegaQrpid( $_GET['acao'], 2, $perfil, $ano );
				$menu = array(
							0 => array("id" => 0, "descricao" => "Programa Escola Acessível – Edição 2008", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=A")
							);
				echo montarAbasArray($menu, "/pdeescola/pdeescola.php?modulo=questionario&acao=A");
			} else {
				echo "<script>
				 alert('Você não possui permissão para visualizar esse questionário!');
				 history.back(-1);
			  </script>";
			}
		}
	}elseif($_GET['acao'] == 'B'){
		$qrpid = pegaQrpid( $_GET['acao'], $_GET['aba'], $perfil, $ano );
		$menu = array(
				//	0 => array("id" => 0, "descricao" => "Programa Educação Inclusiva: direito à diversidade – Edição 2008", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=B&aba=1".$val),
					0 => array("id" => 0, "descricao" => "Programa de Implantação de Salas de Recursos Multifuncionais – Edição 2008", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=B&aba=2".$val),
				//	2 => array("id" => 2, "descricao" => "Programa de Formação de Professores em Educação Especial", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=B&aba=3".$val)
					);
		echo montarAbasArray($menu, "/pdeescola/pdeescola.php?modulo=questionario&acao=B&aba=".$_GET['aba']."".$val);
	}elseif($_GET['acao'] == 'C'){
		$qrpid = pegaQrpid( $_GET['acao'], $_GET['aba'], $perfil, $ano );
		$menu = array(
				//	0 => array("id" => 0, "descricao" => "Direito à diversidade - Edição 2008", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=C&aba=1".$val),
				//	1 => array("id" => 1, "descricao" => "Programa de Formação de Professores em Educação Especial", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=C&aba=2".$val),
				//	2 => array("id" => 2, "descricao" => "Programa de Formação de Professores em Educação Especial", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=C&aba=3".$val),
					0 => array("id" => 0, "descricao" => "Programa de Implantação de Salas de Recursos Multifuncionais – Edição 2008", "link" => "/pdeescola/pdeescola.php?modulo=questionario&acao=C&aba=4".$val)
					);
		echo montarAbasArray($menu, "/pdeescola/pdeescola.php?modulo=questionario&acao=C&aba=".$_GET['aba']."".$val);
	} 

	$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25) );
}

?>
</fieldset>
</center>