<?php
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/Snoopy.class.php";
include "funcoes.php";

# abre conexão com o banco
/*
$nome_bd     = '';
$servidor_bd = '';
$porta_bd    = '5432';
$usuario_db  = '';
$senha_bd    = '';

*/
$db = new cls_banco();
$url = $_SERVER ['REQUEST_URI'];

if ($_REQUEST['estadoescolha']) {
	$estado = $_REQUEST['estado'];
	$listaMunicipios = "SELECT 	muncod as codigo,
								mundescricao as descricao
	                    FROM territorios.municipio
	                    WHERE estuf = '".$estado."'
	                    ORDER BY mundescricao ASC";
	echo $db->monta_combo("muncod", $listaMunicipios,'S' , "Selecione...", '', '', '', '300', 'N', 'muncod');
    die();
}

////////////// Mostra telas ////////////////////
$inuid =  $_REQUEST['inuid'];
$itrid =  $_REQUEST['itrid'];
if($_REQUEST['estuf'] == ''){
	$uf =  $_REQUEST['est'];
}else{
	$uf =  $_REQUEST['estuf'];
}
$ordem = $_REQUEST["ordem"] ? $_REQUEST["ordem"] : 0;

$NomeMunicipios =  $_REQUEST['mun'];

$url = "";
$municod = $_REQUEST['municod'] ? $_REQUEST['municod'] : "";
if($_REQUEST['muncod']){
	$_REQUEST['system'] = "apresentacao";
	$url = "/cte/relatoriopublico/principal.php?system=apresentacao";
	$municod = $_REQUEST['muncod'];
	$NomeMunicipios = "SELECT mundescricao as descricao FROM territorios.municipio WHERE muncod = '".$_REQUEST['muncod']."' ORDER BY estuf";
	$NomeMunicipios =  $db->pegaUm($NomeMunicipios);
	$sqlInuid = "SELECT inuid FROM cte.instrumentounidade WHERE muncod ='".$_REQUEST['muncod']."'  and itrid = 2";
	$inuid = $db->pegaUm($sqlInuid);
 	$uf = "SELECT estdescricao as descricao FROM territorios.estado WHERE estuf = '".$uf."' ORDER BY estdescricao ASC ";
	$uf = $db->pegaUm($uf);
	$_SESSION['muncod'] = $_REQUEST['muncod'];

	if($inuid){
		$sql = "select itrid from cte.instrumentounidade where inuid = '" . $inuid."'" ;
		$itrid = $db->pegaUm($sql);
	}	
	$sqlSituacao = "
					SELECT coalesce(esd.esdordem, 99)
					FROM territorios.municipio mnu
						LEFT JOIN cte.instrumentounidade itr on mnu.muncod = itr.muncod and itr.itrid = 2
						LEFT JOIN workflow.documento doc on itr.docid = doc.docid
						LEFT JOIN workflow.estadodocumento esd on doc.esdid = esd.esdid AND esd.tpdid = 2 
					WHERE mnu.muncod = '". $_REQUEST['muncod'] ."'
					order by esdordem
					limit 1;";
	$ordem = $db->pegaUm( $sqlSituacao );
}

///////////// Monta Combos ////////////////////
$listaEstados = "SELECT estuf as codigo,
                        estuf || ' - ' || estdescricao as descricao
                 FROM territorios.estado
                 ORDER BY estdescricao ASC ";
$estuf = $_REQUEST['estuf']; 
$estado = $db->monta_combo("estuf", $listaEstados, 'S', "Selecione...", 'exibirMunicipios', '', '', '200', 'N', 'estuf',true);

?>
<html>
<head>

<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
<style type="text/css">
body{
	margin-top:0px;
	margin-left:0px;
	margin-right:0px;
	background-position:top;
}

.bandeira{
	width:190px;
	height:35px;
	background-image:url(./imagens/brasil.gif);
	background-repeat:no-repeat;
	background-color:#ffcc00;
	background-position:right; 
}
.meio{
background-color:#ffcc00;
}

.brasil{
	width:120px;
	height:35px;
	background-image:url(./imagens/bandeira.gif);	
	background-repeat:no-repeat;
	background-color:#ffcc00;
	background-position:left; 
}

.select_gov{
	background-color:#ffcc00;
}

</style>
</head>
<body>

<table class="BarraGoverno" width="100%" border="0" cellpadding="0" cellspacing="0">
  <tr>
    <td class="brasil" width="120px">&nbsp;</td>
    <td class="bandeira" width="190">
    <select onchange="nova_janela(this)" class="select_gov" id="Governo" title="Destaques do Governo Federal">			
							<option value="0">Destaques do Governo</option>
							<option value="http://www.brasil.gov.br">Portal de Serviços do Governo</option>

							<option value="http://www.radiobras.gov.br/">Portal da Agência de Notícias</option>
							<option value="http://www.brasil.gov.br/noticias/em_questao">Em Questão</option>
							<option value="http://www.fomezero.gov.br/">Programa Fome Zero</option>
						</select>
		</td>
  </tr>
</table>
<form action="" method="post" name="formulario">
<input type="hidden" id="submetido" name="submetido" value="0">
<input type="hidden" id="ordem" name="ordem" value="0">

<table>	
	<thead>
		<th> 
			<table class="tabela notscreen" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			         <?php if($NomeMunicipios != NULL){ ?>
						<tr>
							<td colspan="5" valign="top" bgcolor="#7e8e47" align="center" style="font-size: 13pt; text-align: center; border-top: 2px solid #d0d0d0; color: #fff;">Relatório Público do Município <i><b><?=$NomeMunicipios;?></b></i> do Estado do <i><b><?=$uf;?></b></i></td>
						</tr>
					 <?php } ?>
			</table>
		</th>
	</thead>
</table>

<table class="tabela notprint" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
    	<td class="SubTituloDireita" valign="top" width="10%">Estado:</td>
        <td  width="20%">
		 <?=$estado?>
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita" valign="top" width="10%">Município:</td>
          <td id="municipio" width="60%">
          	<? if($estuf){ 
				    $estado = $estuf;
					$listaMunicipios = "SELECT 	muncod as codigo,
												mundescricao as descricao
					                    FROM territorios.municipio
					                    WHERE estuf = '".$estado."'
					                    ORDER BY mundescricao ASC";
					$muncod = $municod;
					echo $db->monta_combo("muncod", $listaMunicipios,'S' , "Selecione...", '', '', '', '300', 'N', 'muncod');
          		}else{
          			echo "Selecione um Estado.";		
          		} 
          	?>
          </td>

    </tr>
    <tr>
        <td class="SubTituloDireita" valign="top" width="10%"></td>
        <td>
        <input type='button' class='botao' name='Consultar' value='Consultar' title='Consultar' onclick='submeter();'/>
         <input type='button' class='botao' id="Impressao" name='Impressao' value='Impressão' title='Impressão' onclick='imprimir()'/>
        </td>
    </tr>
         <?php if($NomeMunicipios != NULL){ ?>
			<tr>
				<td colspan="5" valign="top" bgcolor="#7e8e47" align="center" style="font-size: 13pt; text-align: center; border-top: 2px solid #d0d0d0; color: #fff;">Relatório Público do Município <i><b><?=$NomeMunicipios;?></b></i> do Estado do <i><b><?=$uf;?></b></i></td>
			</tr>
		 <?php } ?>
</table>
</form>
<?php
	if($NomeMunicipios != NULL){
		// Nulo (Não iniciado)
		if( $ordem == 99 ){
			echo '<span class="avisoRelatorio">Este Município não iniciou a elaboração do Plano de Ações Articuladas.</span>';
		}     
		// De 5 a 8 (Fase de Análise Financeira em diante)
		elseif( $ordem > 4 ){
			$uf = removeAcentos($uf);
			$NomeMunicipios = removeAcentos($NomeMunicipios);

			$itensMenu = Array(
					Array('Apresentação','principal.php?system=apresentacao&ordem='.$ordem.'&inuid='.$inuid.'&itrid='.$itrid.'&est='.$uf.'&mun='.$NomeMunicipios.'&municod='.$municod.'&estuf='.$estuf.'&muncod='.$muncod,'' ),
					Array('Sintese do indicador do PAR','principal.php?system=indicador&ordem='.$ordem.'&inuid='.$inuid.'&itrid='.$itrid.'&est='.$uf.'&mun='.$NomeMunicipios.'&municod='.$municod.'&estuf='.$estuf,'' ),
					Array('Sintese da dimensão do PAR','principal.php?system=dimensao&ordem='.$ordem.'&inuid='.$inuid.'&itrid='.$itrid.'&est='.$uf.'&mun='.$NomeMunicipios.'&municod='.$municod.'&estuf='.$estuf,''),
//					Array('Sintese das questões pontuais do PAR','principal.php?system=questoespontuais&ordem='.$ordem.'&inuid='.$inuid.'&itrid='.$itrid.'&est='.$uf.'&mun='.$NomeMunicipios.'&municod='.$municod,''),
					Array('Sintese do PAR','principal.php?system=sintesepar&ordem='.$ordem.'&inuid='.$inuid.'&itrid='.$itrid.'&est='.$uf.'&mun='.$NomeMunicipios.'&municod='.$municod.'&estuf='.$estuf,''),
					Array('Termo de Cooperação','principal.php?system=cooperacao&ordem='.$ordem.'&inuid='.$inuid.'&itrid='.$itrid.'&est='.$uf.'&mun='.$NomeMunicipios.'&municod='.$municod.'&estuf='.$estuf,''),
					Array('Liberação de Recursos (FNDE)','principal.php?system=licenciamento&ordem='.$ordem.'&inuid='.$inuid.'&itrid='.$itrid.'&est='.$uf.'&mun='.$NomeMunicipios.'&municod='.$municod.'&estuf='.$estuf,''),
					Array('Indicadores Demográficos e Educacionais','principal.php?system=indicadores&ordem='.$ordem.'&inuid='.$inuid.'&itrid='.$itrid.'&est='.$uf.'&mun='.$NomeMunicipios.'&municod='.$municod.'&estuf='.$estuf,'')
				);
				
			$menu = montarAbasArray2( $itensMenu, $url );
		}
		// 3 ou 4 (Aguardando Validação Local ou Fase de Análise Técnica)
		elseif( $ordem > 2 ){
			echo '<span class="avisoRelatorio">O Município já concluiu o Plano de Ações Articuladas e o MEC está realizando sua análise técnica.</span>';
			echo '<span class="avisoRelatorioPortal">Sr Secretário ou Sr. Prefeito. Favor acessar simec.gov.br para acompanhar o PAR do seu município.</span>';
		}
		// 1 ou 2 (Em Elaboração (Fase de Diagnóstico) ou Em Elaboração PAR)
		elseif( $ordem > 0 ){
			echo '<span class="avisoRelatorio">Este Município já iniciou a elaboração do Plano de Ações Articuladas, porém, ainda não houve a conclusão e o envio para análise do MEC.</span>';
		}
	}
?>
<table >
	<tr><td>&nbsp;&nbsp;</td></tr>
</table>

<?=$menu; ?>
<script type="text/javascript">
document.getElementById('Impressao').style.display  = "none";
function exibirMunicipios(value)
    {
        new Ajax.Request(window.location.href,
                         {
                             parameters: 'estadoescolha=estado&estado=' + value,
                             onComplete: function(e)
                             {
                                 $('municipio').innerHTML = e.responseText;
                             }
                         });
    }
    
  function submeter(){
  		//var submetido = document.getElementById('submetido');
  		var muncod = document.getElementById('muncod').value;
  		if(muncod){
  			//submetido.value = 1;
  			formulario.submit();
  		}else{
  			alert("Selecione um município para consulta.");
  		}
  }
  
  function imprimir(){
  	//var impressao = document.getElementById('impressao').value = 1;
  	//if(impressao == 1){
  		window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/relatoriopublico/impressao.php?inuid=<?=$inuid;?>&itrid=<?=$itrid;?>&est=<?=$uf;?>&mun=<?=$NomeMunicipios;?>&municod=<?=$municod;?>&estuf=<?=$estuf;?>&muncod=<?=$muncod;?>','Impressao','width=800,height=600,scrollbars=1,menubar=1');
  	//}
  }

</script>
<?php

///////////////// indicador
if($inuid == NULL){
?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
				<tr>
				    <td colspan="5" valign="top" class="SubTituloEsquerda">
						<div style=" color: #FF0000"><?=$erro; ?></div> Escolha um Estado e município para consulta.
				 	</td>
				 	</tr>
				 	</table>
<?
}else{
	if( $ordem > 4 && $ordem != 99 ){
			
		switch($_REQUEST['system']) {
		case 'indicador':
			include('indicador.php');
			break; 
		case 'dimensao':
			include('dimensao.php');
			break; 
/*		case 'questoespontuais':
			include('questoespontuais.php');
			break;*/ 
		case 'sintesepar':
			include('sintesepar.php');
			break; 
		case 'cooperacao':
			include('cooperacao.php');
			break; 
		case 'licenciamento':
			
			include('liberacaorecurso.php');
			break; 	
		case 'indicadores':
			include('indicadores.php');
			break; 				
		default:
			//include('apresentacao.php');
			if( $inuid ){
				include('apresentacao.php');
			}else{
				if($erro){
					?>
					<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
						<tr>
						    <td colspan="5" valign="top" class="SubTituloEsquerda">
								<?=$erro; ?> 
							</td>
							</tr>
							</table>
					<? 
				}
			}
		}
	}	
}

if( !$inuid ){
	include('apresentacao.php');
}
?>
<table class="tabela notprint" align="center"  cellspacing="0" cellpadding="0" style="text-align:right; border:none; margin-top:5px;" >
	<tr>
		<td  style="font-weight:bold;">
			SIMEC - Ministério da Educação
		</td>
	</tr>
<script>
if(document.getElementById('muncod') && (<?=$ordem?> > 4 && <?=$ordem?> != 99)){
	document.getElementById('Impressao').style.display = "inline";
}
function nova_janela(obj) {
   if (obj.value!='0'){
      window.open(obj.value);
   }
}
</script>
</body>
</html>
