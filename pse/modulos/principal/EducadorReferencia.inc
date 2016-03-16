<?
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include APPRAIZ . "includes/classes/dateTime.inc";
VerSessao();
$entid = $_SESSION['pse']['entid'];
$espid = $_SESSION['pse']['espid'];

//Pesquisa lista
if($_REQUEST['CarregaER']){
	header('content-type: text/html; charset=ISO-8859-1');
	listaEducadorReferencia($entid);
	exit;
}

//Salvar Educador Referencia
if($_REQUEST['salvarEducadorReferencia']) {
	
	header('content-type: text/html; charset=ISO-8859-1');
	salvarEducadorReferencia($_POST);
	exit;
}

//Exclui Educador Referência
if($_REQUEST['excluiEducador']) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluiEducador($_POST['id']);
	exit;
}

//Altera Educador Referência
if($_REQUEST['alteraEducador']) {
	header('content-type: text/html; charset=ISO-8859-1');
	alteraEducador($_POST['id']);
	exit;
}
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$titulo = "PSE - Programa Saúde na Escola";
monta_titulo( $titulo , '');
$titulo = "Educador Referência";
monta_titulo( $titulo, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<script>

function funcaoEduc(vr){
	document.form.funcEducador.value=vr;
}

</script>
<form method="POST"  name="form" id="form">
<?php Cabecalho($entid,1); ?>
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">	
<!--  ********* CADASTRO DO EDUCADOR REFERENCIA DO PSE NA ESCOLA ******* -->
<tr>
	<td colspan="2" style="text-align: left;"><a name="L2"></a><b><br>1.3 Cadastro do(s) Educador(es) de Referência do PSE na Escola</b></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Nome do Educador:</td>
    <td style="background: rgb(238, 238, 238)">
    	<input type="hidden" name="ideducador" id="ideducador">
    	<?=campo_texto('nomeEducador','S','S','',100,60,'','','','','','id=nomeEducador');?>
    </td>
</tr> 
<tr>
    <td align='right' class="SubTituloDireita">Endereço:</td>
    <td style="background: rgb(238, 238, 238)"><?=campo_texto('enderecoEducador','S','S','',100,60,'','','','','','id=enderecoEducador');?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">CEP (Digite somente números):</td>
    <td style="background: rgb(238, 238, 238)"><?=campo_texto('cepEducador','S','S','',100,9,'','','','','','id=cepEducador onkeypress="return somenteNumeros(event)";',"this.value=mascaraglobal('#####-###',this.value);",''," mascaraglobal('#####-###',this.value);");?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Telefone de Contato:</td>
    <td style="background: rgb(238, 238, 238)"><?=campo_texto('telEducador','S','S','',100,20,'','','','','','id=telEducador');?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">E-mail pessoal:</td>
    <td style="background: rgb(238, 238, 238)"><?=campo_texto('emailEducador','S','S','',100,70,'','','','','','id=emailEducador');?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">RG:</td>
    <td style="background: rgb(238, 238, 238)"><?=campo_texto('rgEducador','S','S','',100,20,'','','','','','id=rgEducador');?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">CPF:</td>
    <td style="background: rgb(238, 238, 238)"><?=campo_texto('cpfEducador','S','S','',100,14,'','','','','','id=cpfEducador onkeypress="return somenteNumeros(event);"',"this.value=mascaraglobal('###.###.###-##',this.value);");?></td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Cargo/Função na escola:</td>
    <td style="background: rgb(238, 238, 238)"><?
			$sql = "SELECT
						cf.cafid AS codigo,
						cf.cafdsc AS descricao
					FROM
						pse.cargofuncao cf
					ORDER BY cf.cafdsc";
			$db->monta_combo("cargoEducador", $sql, 'S', 'Selecione...', 'funcaoEduc', '', '', '215', 'S','cargoEducador');
			
		?><input type="hidden" id="funcEducador" name="funcEducador">
	</td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Outro Cargo:</td>
    <td style="background: rgb(238, 238, 238)"><?=campo_texto('outrocargoeducador','N','S','',100,30,'','','','','','id=outrocargoeducador');?></td>
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
				<input type="button" class="botao" name="btsalvar" value="Salvar" onclick="submeterEducadorReferencia()">
			</td>
		</tr>	
	<?php } ?>
<?PHP }?>
</table>
</form>
<table align="center" class="Tabela" cellpadding="2" cellspacing="1">	
<tr>
	<td colspan='2' style="background: rgb(238, 238, 238)">
		<div id="listaB"><? listaEducadorReferencia($entid); ?></div>
	</td>
</tr>
<?=navegacao('EquipeSaudeFamilia','AtendimentoESF');?>
</table>


<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>	
<script type="text/javascript">

function pesquisaER(){
	var myajax = new Ajax.Request('pse.php?modulo=principal/EducadorReferencia&acao=A', {
		        method:     'post',
		        parameters: 'CarregaER=true',
		        asynchronous: false,
		        onComplete: function (res){
					$('listaB').innerHTML = res.responseText;
		        }
	});
}

function submeterEducadorReferencia() {
		var cep = document.getElementById('cepEducador').value;
		
		if( cep.length > 9 ){
			alert("CEP inválido!");
			return false;
		}

		if	(document.getElementById('nomeEducador').value==''||
			document.getElementById('enderecoEducador').value==''||
			document.getElementById('telEducador').value==''||
			document.getElementById('emailEducador').value==''||
			document.getElementById('cpfEducador').value==''||
			document.getElementById('cepEducador').value==''||
			document.getElementById('cargoEducador').value==''||
			document.getElementById('rgEducador').value==''){
				alert('Todos os campos são de preenchimento obrigatório!');	
		}			
		else {
			cep = document.getElementById('cepEducador').value;
			document.getElementById('cepEducador').value=mascaraglobal('######-###',cep);
			var params = $('form').serialize();
			
			var req = new Ajax.Request('pse.php?modulo=principal/EducadorReferencia&acao=A', {
		        method:     'post',
		        parameters: 'salvarEducadorReferencia=true&'+params,
		        asynchronous: false,
		        onComplete: function (res){
		        	if(res.responseText==1){
						document.form.nomeEducador.value="";
			       		document.form.enderecoEducador.value="";
			       		document.form.telEducador.value="";
			       		document.form.emailEducador.value="";
			       		document.form.rgEducador.value="";
			       		document.form.cpfEducador.value="";
			       		document.form.cepEducador.value="";
			       		document.form.funcEducador.value="";
			       		document.form.outrocargoeducador.value="";
			       		document.form.ideducador.value="";
			       		document.getElementById('cargoEducador').selectedIndex=0;
			       		alert('Operação realizada com sucesso!'); 
			       		
		        	}
		        	/*else
		        		alert(res.responseText);*/
		        },
		        onLoading: function(){
					destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
				}
		       
		  });
    	  pesquisaER();
		}
}

function ExcluirEducador(id){
	var req = new Ajax.Request('pse.php?modulo=principal/EducadorReferencia&acao=A', {
	        method:     'post',
	        parameters: 'excluiEducador=true&id='+id,
	        asynchronous: false,
	        onComplete: function (res){
	        	if(res.responseText==1){
	        		alert('Registro excluído com sucesso!');
	        		pesquisaER();
	        	}
	        	else
	        		alert(res.responseText);
	        },
	        onLoading: function(){
				destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
			}
	  });	
}

function AlterarEducador(id){
	var req = new Ajax.Request('pse.php?modulo=principal/EducadorReferencia&acao=A', {
	        method:     'post',
	        parameters: 'alteraEducador=true&id='+id,
	        asynchronous: false,
	        onComplete: function (res){
	        	dados = res.responseText;
	        	dados = dados.split('|');
	        	document.getElementById('ideducador').value=dados[0];
	        	document.getElementById('cargoEducador').selectedIndex = document.getElementById('cargoEducador').options[dados[1]].value;
	        	document.getElementById('nomeEducador').value=dados[2];
	        	document.getElementById('enderecoEducador').value=dados[3];
	        	document.getElementById('cepEducador').value=dados[4]=mascaraglobal('###.###.###-##',dados[4]);
	        	document.getElementById('telEducador').value=dados[5];
	        	document.getElementById('emailEducador').value=dados[6];
	        	document.getElementById('outrocargoeducador').value=dados[7];
	        	document.getElementById('rgEducador').value=dados[8];
	        	//document.getElementById('cpfEducador').value=dados[9];
	        	document.getElementById('cpfEducador').value=dados[9]=mascaraglobal('###.###.###-##',dados[9]);
	        	//this.value=mascaraglobal('###.###.###-##',this.value);
	        	document.getElementById('funcEducador').value=dados[1];
	        },
	        onLoading: function(){
				destino.innerHTML = '<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>';
			}
	  });	
}


</script>

