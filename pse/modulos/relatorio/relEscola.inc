<?

include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include APPRAIZ . "includes/classes/dateTime.inc";
VerSessao();
$entid = $_SESSION['pse']['entid'];
$espid = $_SESSION['pse']['espid'];

if($_REQUEST['CarregaQD']){
	header('content-type: text/html; charset=ISO-8859-1');
	listaQuantitativo($entid,$_REQUEST['ano'],1);
	exit;
}

$sql = "SELECT espparticipapse2009 from pse.escolapse
		WHERE entid=$entid";
$pse = $db->pegaUm($sql);			

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
echo "<br>";
$titulo = "PSE - Programa Saúde na Escola";
monta_titulo( $titulo , '');
$titulo = "Relatório da Escola";
monta_titulo( $titulo, '' );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<body onload="self.print();">
<form method="POST"  name="formulario">
<?php Cabecalho($entid,1); ?>
<tr>
    <td width="100" align='right' class="SubTituloDireita">Participou do PSE/2009?</td>
    <td width="80%" style="background: rgb(238, 238, 238)">
    	<?php
			$checkNao = "";
			$checkSim = "";
			$pse = ($pse=='n'?'':$pse);
			if($pse!="")
				$pse != "t" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
			if($checkNao == "checked=checked"){
				?><b>Não</b><?php
			}else{
				?><b>Sim</b><?php
			}
		?>
    </td>
</tr>
<tr>
	<td colspan="2" style="text-align: left;" class="SubTituloDireita"><b>ESF - Equipe(s) Saúde da Família/Unidade Básica de Saúde</b></td>
</tr>
</form>
<tr>
	<td colspan='2' style="background: rgb(238, 238, 238)">
		<div id="listaA"><? listaEquipeSaudeFamilia($entid, 1); ?></div>
	</td>
</tr>
<tr>
	<td colspan="2" style="text-align: left;" class="SubTituloDireita"><a name="L2"></a><b><br>Cadastro do(s) Educador(es) e Profissional(is) de Saúde de Referência do PSE na Escola</b></td>
</tr>
<tr>
	<td colspan='2' style="background: rgb(238, 238, 238)">
		<div id="listaB"><? listaEducadorReferencia($entid, 1); ?></div>
	</td>
</tr>
<tr>
	<td colspan="2" style="text-align: left;" class="SubTituloDireita"><b>Quantitativo(s) de estudantes, modalidades, e seus respectivos níveis de ensino.</b></td>
</tr>
<?php if($checkSim == "checked=checked"){?>
<tr>
	<td colspan='2' style="background: rgb(238, 238, 238)">
		<div id="listaD"><? listaQuantitativo($entid,2009,1); ?></div>
	</td>
</tr>
<?php } ?>
<tr>
	<td colspan='2' style="background: rgb(238, 238, 238)">
		<div id="listaD"><? listaQuantitativo($entid,2010,1); ?></div>
	</td>
</tr>
<tr>
	<td colspan='2' style="background: rgb(238, 238, 238)">
		<div id="listaD"><? listaQuantitativo($entid,2011,1); ?></div>
	</td>
</tr>
<tr>
	<td colspan="2" style="text-align: left;" class="SubTituloDireita"><b>Atores que integram a equipe do PSE na Escola.</b></td>
</tr>
<tr>
    <td style="background: rgb(238, 238, 238)" align='right' class="SubTituloDireita">
		<select id="destino" class="combo campoEstilo" size="7" multiple="multiple" name="destino">
			<?php 
			$sql = "SELECT a.atrdsc as descricao
					FROM pse.ator a
					INNER JOIN
						pse.atorequipe b ON b.atrid = a.atrid
					WHERE b.espid=$espid
					ORDER BY atrdsc";
			$arDados = $db->carregar($sql);
			if($arDados<>''){
				foreach($arDados as $dados){
					echo "<option value=\"\">{$dados['descricao']}</option>";
				}	
			}
			?>
		</select>
    </td><td style="background: rgb(238, 238, 238)"></td>
</tr> 
<tr>
	<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='2'>
		<input type="button" class="botao" name="btvoltar" value="Voltar" onclick="voltarpage()">
	</td>
</tr>
</table>


<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">

function gravapse2009() {
	document.formulario.submit();

}
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
function pesquisaQD(ano){
	var myajax = new Ajax.Request('pse.php?modulo=relatorio/relEscola&acao=A', {
		        method:     'post',
		        parameters: 'CarregaQD=true&ano='+ano,
		        asynchronous: false,
		        onComplete: function (res){
					$('listaD').innerHTML = res.responseText;
		        }
		  });
}
function voltarpage(){
	window.location.href='pse.php?modulo=principal/cadastroEstadoMunicipioArvore&acao=A';
}

document.getElementById('origem').disabled = true
document.getElementById('destino').disabled = true

</script>