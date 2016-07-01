<?
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include APPRAIZ . "includes/classes/dateTime.inc";
VerSessao();
$entid = $_SESSION['pse']['entid'];
$espid = $_SESSION['pse']['espid'];

//Equipe vinculadas no CNES informado
if($_REQUEST['verEquipesAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	verEquipesAjax($_REQUEST['cnes'],$entid,$_REQUEST['equipe']);
	exit;
}

//Lista Equipe Saude Familia
if($_REQUEST['CarregaESF']){
	header('content-type: text/html; charset=ISO-8859-1');
	listaEquipeSaudeFamilia($entid);
	exit;
}

//Salvar Equipe Saúde Família
if($_REQUEST['salvarEquipeSaudeFamilia']) {
	header('content-type: text/html; charset=ISO-8859-1');
	salvarEquipeSaudeFamilia($_POST);
	exit;
}

//Exclui Equipe Saúde Família
if($_REQUEST['excluiEquipeSaudeFamilia']) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluiEquipeSaudeFamilia($_POST['id']);
	exit;
}

//Altera Equipe Saúde Família
if($_REQUEST['alteraEquipeSaudeFamilia']) {
	header('content-type: text/html; charset=ISO-8859-1');
	alteraEquipeSaudeFamilia($_POST['id']);
	exit;
}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$titulo = "PSE - Programa Saúde na Escola";
monta_titulo( $titulo , '');
$titulo = "Equipe Saúde da Família";
monta_titulo( $titulo, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>

<form method="POST"  name="formulario">
<?php Cabecalho($entid,1); ?>

<!--  ********* EQUIPES SAÚDE NA ESCOLA ******* -->
<tr>
	<td colspan="2" style="text-align: left;"><b>1.2 ESF - Equipe(s) Saúde da Família/Unidade Básica de Saúde</b></td>
</tr>
<tr>
   	<td width="100" align='right' class="SubTituloDireita">Código do SCNES:</td>
   	<td width="80%" style="background: rgb(238, 238, 238)">
   		<input type="hidden" name="esfid" id="esfid" value="<?=$espid;?>">
   		<input type="hidden" name="idequipe" id="idequipe">
   		<input type="hidden" name="idcnes" id="idcnes">
   		<?=campo_texto('codSCNES','S','N','',15,10,'','','','','','id=codSCNES');?>
   		<a href="#" onclick="consultarSCNES()"><img src="../imagens/consultar.gif" id="imgcnes" title="Consultar código SCNES" style="cursor:pointer;" border="0"></a>
   </td>
</tr>
<tr>
   	<td align='right' class="SubTituloDireita">Nº da Equipe:</td>
   	<td style="background: rgb(238, 238, 238)" id="colequipe">
   		<?=campo_texto('nrequip','S','N','',10,10,'','','','','','');?>
   	</td>
</tr> 
<tr>
   	<td align='right' class="SubTituloDireita">Endereço da Equipe:</td>
   	<td style="background: rgb(238, 238, 238)" ><?=campo_texto('endequipe','S','S','',100,60,'','','','','','id=endequipe');?></td>
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
				<input type="button" class="botao" name="btsalvar" value="Salvar" onclick="SubmeteEquipeSaudeFamilia()">
			</td>
		</tr>	
	<?php } ?>
<?PHP }?>	
</form>
<tr>
	<td colspan='2' style="background: rgb(238, 238, 238)">
		<div id="listaA"><? listaEquipeSaudeFamilia($entid); ?></div>
	</td>
</tr>
<?=navegacao('CadastroEscola','EducadorReferencia');?>
</table>



<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>	
<script type="text/javascript">

function verEquipes(cnes,equipe) {
		var req = new Ajax.Request('pse.php?modulo=principal/EquipeSaudeFamilia&acao=A', {
		        method:     'post',
		        parameters: 'verEquipesAjax=true&cnes='+cnes+'&equipe='+equipe,
		        asynchronous: false,
		        onComplete: function (res){
		        	$('colequipe').innerHTML = res.responseText;
		        },
		        onLoading: function(){
					destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
				}
		  });
}

function SubmeteEquipeSaudeFamilia() {
		/*if	(document.getElementById('idequipe').value==''||
			 document.getElementById('idcnes').value==''||
			 document.getElementById('endequipe').value==''){
				alert('Todos os campos são de preenchimento obrigatório!');	
		}*/
		//else {
		var nequipe = document.getElementById('nrequip').options[document.getElementById('nrequip').selectedIndex].value;
		var params = 'idcnes=' + nequipe + '&endequipe=' + $F('endequipe') + '&entid='+ <?=$entid;?> + '&idequipe=' + $F('idequipe') + '&espid=' + <?=$espid;?>;

		var req = new Ajax.Request('pse.php?modulo=principal/EquipeSaudeFamilia&acao=A', {
		        method:     'post',
		        parameters: 'salvarEquipeSaudeFamilia=true&'+params,
		        asynchronous: false,
		        onComplete: function (res){
		        	if(res.responseText==1){
		       			document.formulario.codSCNES.value="";
		       			document.formulario.endequipe.value="";
		       			document.formulario.idequipe.value="";
		       			document.formulario.idcnes.value="";
		       			verEquipes(0);
						alert('operação realizada com sucesso!');
						document.getElementById('imgcnes').style.display = '';
		        		pesquisaESF();
		        	}
		        	else
		        		alert(res.responseText);
		        },
		        onLoading: function(){
					destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
				}
		       
		  });
		//}
}

function pesquisaESF(){
	var myajax = new Ajax.Request('pse.php?modulo=principal/EquipeSaudeFamilia&acao=A', {
		        method:     'post',
		        parameters: 'CarregaESF=true',
		        asynchronous: false,
		        onComplete: function (res){
					$('listaA').innerHTML = res.responseText;
					identificaESF();
		        }
	});
}

function ExcluirEquipe(id){
	var req = new Ajax.Request('pse.php?modulo=principal/EquipeSaudeFamilia&acao=A', {
	        method:     'post',
	        parameters: 'excluiEquipeSaudeFamilia=true&id='+id,
	        asynchronous: false,
	        onComplete: function (res){
	        	if(res.responseText==1){
	        		alert('Registro excluído com sucesso!');
	        		pesquisaESF();
	        	}
	        	else
	        		alert(res.responseText);
	        },
	        onLoading: function(){
				destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
			}
	  });	
}

function AlterarEquipe(id){
	var req = new Ajax.Request('pse.php?modulo=principal/EquipeSaudeFamilia&acao=A', {
	        method:     'post',
	        parameters: 'alteraEquipeSaudeFamilia=true&id='+id,
	        asynchronous: false,
	        onComplete: function (res){
	        	dados = res.responseText;
	        	dados = dados.split('|');
	        	document.getElementById('idcnes').value=dados[0];
	        	document.getElementById('codSCNES').value=dados[1];
	        	document.getElementById('endequipe').value=dados[2];
	        	document.getElementById('idequipe').value=dados[3];
	        	document.getElementById('imgcnes').style.display = 'none';
	        	verEquipes(dados[1],dados[0]);
	        },
	        onLoading: function(){
				destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
			}
	  });	
}
function consultarSCNES() {
		window.open('pse.php?modulo=principal/popConsultaSCNES&acao=A','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=400,height=500');
}
</script>



