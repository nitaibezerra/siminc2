<?
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include APPRAIZ . "includes/classes/dateTime.inc";
VerSessao();
$entid = $_SESSION['pse']['entid'];
$espid = $_SESSION['pse']['espid'];
$muncod = pegaMuncodEscola( $entid );
$portaria = pegaPortaria( $muncod );
$anoMax = $portaria + 3;
$habilitado = 'S';

$novoEspid = pegaNovoEspid( $entid, $portaria );
//ver(1,1,1,1,$espid, $novoEspid);
if( $espid != $novoEspid && $novoEspid != '' ){
	//$habilitado = 'N';
}
//ver (a,a,a,a,a,a,a,$_REQUEST);
//Combo de ESF
/*if($_REQUEST['CarregaidentESF']){
	header('content-type: text/html; charset=ISO-8859-1');
	identificaESF($entid);
	exit;
}*/

//Salvar Quantitativo
if($_REQUEST['SalvarQuantitativo']){
	header('content-type: text/html; charset=ISO-8859-1');
	salvarQuantitativo($post);
	exit;
}

//Pesquisa lista
if($_REQUEST['CarregaQD']){
	header('content-type: text/html; charset=ISO-8859-1');
	listaQuantitativo($entid,$_REQUEST['ano']);
	exit;
}

//Exclui registro da lista
if($_REQUEST['excluiAtendimento']){
	header('content-type: text/html; charset=ISO-8859-1');
	excluiAtendimento($_REQUEST['id']);
	exit;
}

//Altera registro da lista
if($_REQUEST['alteraAtendimento']){
	header('content-type: text/html; charset=ISO-8859-1');
	alteraAtendimento($_REQUEST['id']);
	exit;
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";

$titulo = "PSE - Programa Saúde na Escola";
monta_titulo( $titulo, '' );
$titulo = "Atendimentos ESF";
monta_titulo( $titulo, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script>
/*
function modalensino(vr){
	document.formulario.modalidadeEnsino.value=vr;
}
function nivensino(vr){
	document.formulario.nensino.value=vr;
}
function iesf(vr){
	document.formulario.idesf.value=vr;
}
*/
</script>

<form method="POST"  name="formulario" id="formulario">
<input type="hidden" id="paeid" name="paeid">
<input type="hidden" id="espid" name="espid" value="<?=$espid ?>">
<input type="hidden" id="novoespid" name="novoespid" value="<?=$novoEspid ?>">
<input type="hidden" value='<?=$_SESSION['exercicio'] ?>' id="exercicio" name="exercicio">
<?php Cabecalho($entid,1); ?>

<!--  *********  INDICAÇÃO  DE QUANTITATIVO ******* -->
<tr>
    <td align='right' class="SubTituloDireita">Selecione o Ano de Referência:</td>
    <td style="background: rgb(238, 238, 238)">
    	 <?php
		if( $_SESSION['exercicio'] != $anoMax ){	
	    	$nomeVar = "select".($_SESSION['exercicio']);
			${$nomeVar} = ($_REQUEST['ano'] == ($_SESSION['exercicio'])) ? "selected=selected" : "";
		}
		if( ($_SESSION['exercicio'] + 1) != $anoMax ){	
	    	$nomeVar2 = "select".($_SESSION['exercicio'] + 1);
			${$nomeVar2} = ($_REQUEST['ano'] == ($_SESSION['exercicio'] + 1)) ? "selected=selected" : "";
		}
		if( ($_SESSION['exercicio'] + 2) != $anoMax ){	
	    	$nomeVar3 = "select".($_SESSION['exercicio'] + 2);
			${$nomeVar3} = ($_REQUEST['ano'] == ($_SESSION['exercicio'] + 2)) ? "selected=selected" : "";
		}
	 	?>
	    
	    	<select name='anoref' id='anoref' size='1' onchange='pesquisaQD(this.value);regras(this.value);'>
    			<option value=''>Selecione...</option>
			    <?php
				if( $_SESSION['exercicio'] != $anoMax ){ ?>
	   				<option value='<?=$_SESSION['exercicio'] ?>' <?=${$nomeVar} ?>><?=$_SESSION['exercicio'] ?></option>
				<?php }
				if( ($_SESSION['exercicio'] + 1) != $anoMax ){ ?>	
	   				<option value='<?=$_SESSION['exercicio'] + 1 ?>' <?=${$nomeVar2} ?>><?=$_SESSION['exercicio'] + 1 ?></option>
				<?php }
				if( ($_SESSION['exercicio'] + 2) != $anoMax ){ ?>	
	   				<option value='<?=$_SESSION['exercicio'] + 2 ?>' <?=${$nomeVar3} ?>><?=$_SESSION['exercicio'] + 2 ?></option>
				<?php }
			 	?>
    		</select><img border='0' src='../imagens/obrig.gif'/>
    </td>
</tr>
<tr>
	<td colspan="2" style="text-align: left;"><b>2. Quantitativo(s) de estudantes, modalidades, e seus respectivos níveis de ensino.</b></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Identificação da ESF:</td>
     <td style="background: rgb(238, 238, 238)"><?
     	if( $espid == $novoEspid ){
     		$where2 = " and es.espano = ".$_SESSION['exercicio'];
     	}
			$sql = "SELECT	esf.esfid as codigo,
							sc.cnecodigocnes||' - '||'Equipe '||sc.cneseqequipe||' - '||sc.cnenomefantasia as descricao
					FROM	pse.equipesaudefamilia esf, pse.scnes sc, pse.escolapse es
					WHERE	esf.cneid = sc.cneid and esf.espid=es.espid and es.espid = {$espid} and es.entid = {$entid} {$where2}"; 
			//ver($sql);
			$db->monta_combo('codequipe', $sql, 'S', 'Selecione...', '', '', '', '450', 'S','codequipe');
		?>
	</td>
</tr> 
<tr>
    <td align='right' class="SubTituloDireita">Modalidade de Ensino:</td>
     <td style="background: rgb(238, 238, 238)"><?
			//$muncod = $_REQUEST['muncod'];
			$sql = "SELECT
						mod.moeid AS codigo,
						mod.moedsc AS descricao
					FROM
						pse.modalidadeensino mod 
					ORDER BY mod.moedsc"; 
			$db->monta_combo( "modEnsino", $sql, 'S', 'Selecione...', '', '', '', '450', 'S','modEnsino');
		?>
	</td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Nível de ensino:</td>
     <td style="background: rgb(238, 238, 238)"><?
			//$muncod = $_REQUEST['muncod'];
			$sql = "SELECT
						niv.nieid AS codigo,
						niv.niedsc AS descricao
					FROM
						pse.nivelensino niv 
					--ORDER BY niv.niedsc"; 
			$db->monta_combo( "nivelEnsino", $sql, 'S', 'Selecione...', '', '', '', '450', 'S','nivelEnsino');
		?>
	</td>
</tr>
<tr id="realizado">
    <td align='right' class="SubTituloDireita">Quantitativo de estudantes Previstos
    <td style="background: rgb(238, 238, 238)"><?=campo_texto('quantEstPrev','S',$habilitado,'',20,7,'','','','','','id=quantEstPrev onkeypress="return somenteNumeros(event);"');?></td>
</tr>
<tr id="atendidos">
    <td align='right' class="SubTituloDireita">Quantitativo de estudantes Atendidos:</td>
    <td style="background: rgb(238, 238, 238)"><?=campo_texto('quantEstAten','S','S','',20,7,'','','','','','id=quantEstAten onkeypress="return somenteNumeros(event);"');?></td>
</tr>
<?php if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){?>
<?php 
	$sql = "SELECT * FROM pse.programacaoexercicio WHERE prsano = '".$_SESSION['exercicio']."'";
	$arr = $db->pegaLinha( $sql );
	$dataAtual = date("Y-m-d");
	$data = new Data();
	$dataF = trim($arr['prsdata_termino']);
	$resp = 1;
		if( !empty($dataF) ){
			$resp = $data->diferencaEntreDatas($dataAtual, $arr['prsdata_termino'], 'maiorDataBolean','','');		
		}
	if( $resp == NULL ){ ?>
		<tr bgcolor="#cccccc">
			<td style="text-align: center" colspan='2'>
				<input type="button" class="botao" name="btsalvar" value="Salvar" onclick="submeterQuantitativo()">
			</td>
		</tr>
	<?php } ?>
<?php }?>
</form>
<tr>
	<td colspan='2' style="background: rgb(238, 238, 238)">
		<div id="listaD"></div>
	</td>
</tr>
<?=navegacao('EducadorReferencia','AtoresPSE');?>
</table>



<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>	
<script type="text/javascript">

$('atendidos').hide();

function regras(ano){
	if(ano=='<?=$_SESSION['exercicio'] ?>') {
		$('atendidos').show();
		$('realizado').hide();
	} else {
		$('realizado').show();
		$('atendidos').hide();
	}
}

function submeterQuantitativo() {
	
	var equip = document.getElementById('codequipe').value;
	var modal = document.getElementById('modEnsino').value;
	var nivel = document.getElementById('nivelEnsino').value;
	var params ='idesf=' + equip + '&modalidadeEnsino=' + modal + 
				'&nensino=' + nivel + '&quantEstPrev=' + $F('quantEstPrev') + 
				'&quantEstAten=' + $F('quantEstAten') +	'&entid=' + <?=$entid;?> +
				'&ano=' + $F('anoref') + '&paeid=' + $F('paeid'); 
	if(document.getElementById('anoref').value == document.getElementById('exercicio').value){
		if	(document.getElementById('anoref').value==''||
		(document.getElementById('anoref').value==2009 && document.getElementById('quantEstAten').value=='')||
		document.getElementById('codequipe').value==''||
		document.getElementById('modEnsino').value==''||
		document.getElementById('nivelEnsino').value==''){
			alert('Todos os campos são de preenchimento obrigatório!');
			return false;
		}	
	} else {
		if	(document.getElementById('anoref').value==''||
		(document.getElementById('anoref').value==2009 && document.getElementById('quantEstAten').value=='')||
		document.getElementById('codequipe').value==''||
		document.getElementById('modEnsino').value==''||
		document.getElementById('nivelEnsino').value==''){
			alert('Todos os campos são de preenchimento obrigatório!');
			return false;
		}	
	}			
		var req = new Ajax.Request('pse.php?modulo=principal/AtendimentoESF&acao=A', {
	        method:     'post',
	        parameters: 'SalvarQuantitativo=true&'+params,
	        asynchronous: false,
	        onComplete: function (res){
	        	if(res.responseText==1){
		       		document.formulario.paeid.value="";
//		       		document.formulario.modalidadeEnsino.value="";
//		       		document.formulario.nensino.value="";
		       		document.formulario.quantEstPrev.value="";
		       		document.formulario.quantEstAten.value="";
		       		document.getElementById('codequipe').selectedIndex=0;
		       		document.getElementById('modEnsino').selectedIndex=0;
		       		document.getElementById('nivelEnsino').selectedIndex=0;
		       		alert('Quantitativos inseridos com sucesso!');
		        	pesquisaQD($F('anoref'));
	        	}
	        	else
	        		alert('Operação não realizada. Favor incluir uma ESF.');
	        },
	        onLoading: function(){
				destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
			}
	       
	  });
}

function pesquisaQD(ano){
		var myajax = new Ajax.Request('pse.php?modulo=principal/AtendimentoESF&acao=A', {
			        method:     'post',
			        parameters: 'CarregaQD=true&ano='+ano,
			        asynchronous: false,
			        onComplete: function (res){
						$('listaD').innerHTML = res.responseText;
			        }
			  });
}

function Alterarquant(id){
	var req = new Ajax.Request('pse.php?modulo=principal/AtendimentoESF&acao=A', {
		method:     'post',
		parameters: 'alteraAtendimento=true&id='+id,
		asynchronous: false,
		onComplete: function (res){
			dados = res.responseText;
			dados = dados.split('|');
			document.getElementById('paeid').value=dados[0];
			document.getElementById('quantEstPrev').value=dados[4];
			document.getElementById('quantEstAten').value=dados[5];
			
			if( dados[6] != <?=$espid ?> && dados[6] != '' ){
				document.getElementById('quantEstPrev').disabled = true;
			}
			
			var select = new Array();
			select[1] = 'codequipe'; 
			select[2] = 'modEnsino'; 
			select[3] = 'nivelEnsino';
			
			var i = 1;
			while( i <= select.length ){
				var elemento = document.getElementById(select[i]);
				for (a=0; a < elemento.options.length; a++){
					if (dados[i] == elemento.options[a].value){
						elemento.selectedIndex = a;
						continue;	
					}
				}
				i++;
			}
			
			
//			document.getElementById('codequipe').selectedIndex= document.getElementById('codequipe').options[dados[1]].value;
//			document.getElementById('modEnsino').selectedIndex=document.getElementById('modEnsino').options[dados[2]].value;
//			document.getElementById('nivelEnsino').selectedIndex=document.getElementById('nivelEnsino').options[dados[3]].value;
			
		},
		onLoading: function(){
			destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
		}
	});	
}



function Excluirquant(id){
	ano = document.getElementById('anoref').options[document.getElementById('anoref').selectedIndex].value;
	var req = new Ajax.Request('pse.php?modulo=principal/AtendimentoESF&acao=A', {
	        method:     'post',
	        parameters: 'excluiAtendimento=true&id='+id,
	        asynchronous: false,
	        onComplete: function (res){
	        	if(res.responseText==1){
	        		alert('Registro excluído com sucesso!');
	        		pesquisaQD(ano);
	        	}
	        	else
	        		alert(res.responseText);
	        },
	        onLoading: function(){
				destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
			}
	  });	
}
</script>

