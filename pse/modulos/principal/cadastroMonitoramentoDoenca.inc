<?php

//recupera o entid
$entid = $_SESSION['pse']['entid'];

//cria sessao id da avaliacao
$_SESSION['pse']['avlid'] = $_REQUEST['avlid'] ? $_REQUEST['avlid'] : $_SESSION['pse']['avlid'];


if(!$_SESSION['pse']['avlid'] || !$_SESSION['pse']['entid'] || !$_SESSION['pse']['comid'] || !$_SESSION['pse']['acaoid']){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente!");
		location.href='pse.php?modulo=principal/cadastroMonitoramento&acao=A';
	</script>
	<?
	exit;
}


//verifica permissão
$vpflcod = pegaPerfil($_SESSION['usucpf']);
if($vpflcod == SUPER_USUARIO || $vpflcod == ESCOLA_MUNICIPAL || $vpflcod == ESCOLA_ESTADUAL){
	$vPermissao = 'S';
	
	//verifica se a avaliacao foi validada
	/*
	$sql = "select avlvalidar from pse.avaliacao where avlid = ".$_SESSION['pse']['avlid'];
	$avlvalidar = $db->pegaUm($sql);
	if($avlvalidar == 'S') $vPermissao = 'N';
	*/
	

}
else{
	$vPermissao = 'N';
}

//apos 31 de novembro e diferente de monitoramento de ações, permite so vizualizar os dados
if( date('Ymd') > 20121130 && $_SESSION['pse']['sse'] != 'N' ) $vPermissao = 'N';




//submit
if($_POST){
	
		
	$sql = "DELETE FROM pse.educandodoenca WHERE avlid = ".$_SESSION['pse']['avlid'];
	$db->executar($sql);					
	
	if($_POST['doenca']){
		
		foreach($_POST['doenca'] as $i => $v){
			if($v){
				$sql = "INSERT INTO pse.educandodoenca (avlid,doeid,eddquantidade)
						VALUES (".$_SESSION['pse']['avlid'].",".$i.",".$v.")";	
				$db->executar($sql);
			}
		}
	}
	
	
	$db->commit();
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroMonitoramentoDoenca&acao=A&avlid={$_SESSION['pse']['avlid']}';
		   </script>";
	exit();
	
}






// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';


echo "<br>";

/*
$menu = array(0 => array("id" => 1, "descricao" => "Termo de Compromisso", 			"link" => "/pse/pse.php?modulo=principal/cadastroTermoCompromisso&acao=A"),
			  1 => array("id" => 2, "descricao" => "Monitoramento de Ações", 	"link" => $_SERVER['REQUEST_URI']),
			  2 => array("id" => 3, "descricao" => "Semana Saúde na Escola", 	"link" => "/pse/pse.php?modulo=principal/cadastroTermoSemana&acao=A")
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
*/

$titulo = "Monitoramento das Ações do PSE na Escola";
$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';

monta_titulo( $titulo, $dsctitulo );

Cabecalho($entid,1);

echo "<br>";

//$db->cria_aba( $abacod_tela, $url, '');

$menu = array(0 => array("id" => 1, "descricao" => "Lista de Ações", 			"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramento&acao=A"),
			  1 => array("id" => 2, "descricao" => "Avaliações", 	"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramentoAcao&acao=A"),
			  2 => array("id" => 3, "descricao" => "Monitora Doenças", 	"link" => $_SERVER['REQUEST_URI']),
			  //3 => array("id" => 4, "descricao" => "Questionário", 	"link" => "/pse/pse.php?modulo=principal/cadastroQuestionario&acao=A")
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);



//pega total de educandos para as doencas
$sql = "select avlidentifica1 from pse.avaliacao where avlid = ".$_SESSION['pse']['avlid'];
$totalDoenca = $db->pegaUm($sql);
if(!$totalDoenca) $totalDoenca = 0;

?>


<form id="formulario" name="formulario" action="" method="post">


<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td >
		<table border="0" width="100%" >
			<tr>
				<td class="SubTituloCentro" valign="top" align="center">
					 <br>
					 <font color="red">
				     <b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; O cadastro escola deve ser preenchido pelos diretores ou por representantes do PSE na escola,</b>
				     <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; por este indicado e/ou por membros das Equipes de Saúde da Família. <br>
				     </font>
				     <br>
				</td>
			</tr>
	    </table>
	     
	</td>
</tr>
<tr>
	<td style="background: rgb(168, 168, 168);" colspan='3'>
		 <center>
		 <span style="font-size: 12">
		 	<b>COMPONENTE I - AVALIAÇÃO CLÍNICA E PSICOSOCIAL</b>
		 </span>
		 </center>
	</td>
</tr>
<tr>
	<td align="center" style="background-color: #ffffff">
		<table width="100%" align="center" cellpadding="2" cellspacing="2">
			<tr>
				<td colspan='4'  class="SubTituloEsquerda">
					<b>
						<?=$db->pegaUm("select acaodescricao from pse.acao where acaoid = ".$_SESSION['pse']['acaoid']);?>	
					</b>
				</td>
			</tr>
		</table>
	</td>
</tr>
</table>

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='4'>
		 	<center>
			 <span style="font-size: 12"><b>INFORME O NÚMERO DE EDUCANDOS IDENTIFICADOS COM POSSÍVEIS SINAIS DE:</b></span>
			 </center>
		</td>
	</tr>

	<tr>
		<td width="40%" class="SubTituloDireita" colspan='2'>Data da avaliação:</td>
		<td colspan='2'>
			<?=$db->pegaUm("select to_char(avldataavaliacao::date, 'DD/MM/YYYY') from pse.avaliacao where avlid = ".$_SESSION['pse']['avlid']);?>
		</td>
	</tr>
	<?
	$sql = "select doeid, doedescricao from pse.doenca";
	$doencas = $db->carregar($sql);
	foreach($doencas as $d){
		?>
		<tr>
			<td width="50%" class="SubTituloDireita" colspan='2'><?=$d['doedescricao']?>:</td>
			<td colspan='2'>
				<?
					$vldoenca = $db->pegaUm("select eddquantidade from pse.educandodoenca where avlid = {$_SESSION['pse']['avlid']} and doeid = {$d['doeid']}");
					echo campo_texto( 'doenca['.$d["doeid"].']', 'N', $vPermissao, '', 2, 3, '###', '', '', '', 0, "id='txtdoenca'", '', $vldoenca);
				?>
			</td>
		</tr>
	<?}?>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvarC1" id="btSalvarC1" value="Salvar" onclick="validaFormDoe();">
						<?}?>
						<?if($vPermissao == 'S'){?>
							&nbsp;
							<input type="button" class="botao" name="btExcluirC1" id="btExcluirC1" value="Excluir" onclick="excluiFormDoe();">
						<?}?>
						&nbsp;
						<input type="button" class="botao" name="btVoltarC1" id="btVoltarC1" value="Voltar" onclick="location.href='pse.php?modulo=principal/cadastroMonitoramentoAcao&acao=A';">
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
</table>


</form>

</body>
</html>

 
<script>
 
function validaFormDoe(){

	var d = document.formulario;
	var campo = document.getElementsByTagName("input");
	var soma = 0;
	var totalDoenca = "<?=$totalDoenca?>";
	
	for(i=0; i < campo.length; i++){
		if(campo[i].id == 'txtdoenca'){
			if(campo[i].value){
				soma += parseFloat(campo[i].value);
			}
		}
	}
	
	if( parseFloat(totalDoenca) != parseFloat(soma) ){
		alert('A soma de educandos com possíveis sinais deve ser igual a '+totalDoenca);
		return false;
	}
		
	d.submit();

}

function excluiFormDoe(){
	
	var campo = document.getElementsByTagName("input");
	
	if(confirm('Deseja realmente excluir os dados desta tela?')){
		
		for(i=0; i < campo.length; i++){
			if(campo[i].id == 'txtdoenca'){
				if(campo[i].value){
					campo[i].value = '';
				}
			}
		}
		
		document.formulario.submit();
	}
}

</script>