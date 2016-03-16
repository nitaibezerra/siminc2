<?php

//recupera o entid
$entid = $_SESSION['pse']['entid'];

if(!$_SESSION['pse']['entid'] || !$_SESSION['pse']['comid'] || !$_SESSION['pse']['acaoid']){
	?>
	<script>
		alert("Sua sessão expirou. Por favor, entre novamente!");
		location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?
	exit;
}

//recupera o idebp
$dadosEscola = $db->pegaLinha("select idebp, ideqtcrechepac, ideqtpreescolapac, ideqtfundamentalpac,
							   ideqtmediopac, ideqtejapac, ede.muncod, ede.estuf
							   from pse.escolabasepse esc
							   inner join entidade.entidade ent on ent.entcodent = esc.idecodinep
							   left join entidade.endereco ede on ede.entid = ent.entid
							   where ent.entid = {$entid}");
if($dadosEscola) extract($dadosEscola);
 
if(!$idebp){
	?>
	<script>
		alert("Essa Escola não possui acesso ao Componente II.");
		location.href='pse.php?modulo=inicio&acao=C';
	</script>
	<?
	exit;
}



//verifica permissão
$vpflcod = pegaPerfil($_SESSION['usucpf']);
if($vpflcod == SUPER_USUARIO || $vpflcod == EDUCADOR_ESCOLA){
	$vPermissao = 'S';
}
else{
	$vPermissao = 'N';
}

//apos 31 de novembro e diferente de monitoramento de ações, permite so vizualizar os dados
//if( date('Ymd') > 20121130 && $_SESSION['pse']['sse'] != 'N' ) $vPermissao = 'N';



//submit
if($_POST){
	
	if(!$_POST['idebp']){
		?>
		<script>
			alert("Sua sessão expirou. Por favor, entre novamente!");
			location.href='pse.php?modulo=inicio&acao=C';
		</script>
		<?
		exit;	
	}
	
	$sql = "DELETE FROM pse.questionarioescola WHERE idebp = ".$_POST['idebp'];
	$db->executar($sql); 
		
	$sql = "INSERT INTO pse.questionarioescola
			(
            	idebp,
			  	queopcao1,
			  	queopcao2,
			  	queopcao3,
			  	queopcao4,
			  	queopcao5,
			  	queopcao6,
			  	queopcao7,
			  	queopcao8,
			  	queppp 
	        ) VALUES (
	        	".$_POST['idebp'].",
	        	'".($_POST['queopcao1'] == 't' ? 't' : 'f')."',
	        	'".($_POST['queopcao2'] == 't' ? 't' : 'f')."',
	        	'".($_POST['queopcao3'] == 't' ? 't' : 'f')."',
	        	'".($_POST['queopcao4'] == 't' ? 't' : 'f')."',
	        	'".($_POST['queopcao5'] == 't' ? 't' : 'f')."',
	        	'".($_POST['queopcao6'] == 't' ? 't' : 'f')."',
	        	'".($_POST['queopcao7'] == 't' ? 't' : 'f')."',
	        	'".($_POST['queopcao8'] == 't' ? 't' : 'f')."',
	        	'".($_POST['queppp'] == 't' ? 't' : 'f')."'
	        )";	
	$db->executar($sql); 
		
	$db->commit();
	
	unset($_POST);
	
	print "<script>
			alert('Operação realizada com sucesso!');
			location.href='pse.php?modulo=principal/cadastroComp22&acao=A';
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


if($_SESSION['pse']['sse'] == 'S'){
	$titulo = "Monitoramento das Ações da Semana Saúde na Escola";
}elseif($_SESSION['pse']['sse'] == 'N'){
	$titulo = "Monitoramento das Ações do PSE na Escola";
}elseif($_SESSION['pse']['sse'] == 'B'){
	$titulo = "Monitoramento das Ações do Brasil Carinhoso";
}

$dsctitulo = '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';

monta_titulo( $titulo, $dsctitulo );

Cabecalho($entid,1);

echo "<br>";

//$db->cria_aba( $abacod_tela, $url, '');

$menu = array(0 => array("id" => 1, "descricao" => "Lista de Ações", 			"link" => "/pse/pse.php?modulo=principal/monitoraComp2&acao=A"),
			  1 => array("id" => 2, "descricao" => "Avaliações", 	"link" => $_SERVER['REQUEST_URI'])
			  //2 => array("id" => 3, "descricao" => "Ficha de Acompanhamento", 	"link" => "/pse/pse.php?modulo=principal/cadastroMonitoramentoFicha&acao=A"),
			  //3 => array("id" => 4, "descricao" => "Questionário", 	"link" => "/pse/pse.php?modulo=principal/cadastroQuestionario&acao=A")
		  	  );
		
echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);


//carrega dados para edição
if($idebp){
	$sql = "SELECT  
				idebp,
			  	queopcao1,
			  	queopcao2,
			  	queopcao3,
			  	queopcao4,
			  	queopcao5,
			  	queopcao6,
			  	queopcao7,
			  	queopcao8,
			  	queppp 
		     FROM 
		     		pse.questionarioescola
		     WHERE 
		     		idebp = {$idebp}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
}



?>

<script src="../includes/calendario.js"></script>


<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="idebp" value="<?=$idebp?>">

<input type="hidden" name="estuf" value="<?=$estuf?>">
<input type="hidden" name="muncod" value="<?=$muncod?>">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
<tr>
	<td >
		<table border="0" width="100%" >
			<tr>
				<td style="background: rgb(168, 168, 168);">&nbsp;</td>
			</tr>
			<tr>
				<td class="SubTituloCentro" valign="top" align="center">
					 <br>
					 <font color="red">
				     <b> 
				     	PROJETO POLÍTICO PEDAGÓGICO
				     	<br>
				     </b>
				     </font>
				     <br>
				</td>
			</tr>
			<tr>
				<td style="background: rgb(168, 168, 168);">&nbsp;</td>
			</tr>
	    </table>
	</td>
</tr>
</table>

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
	</tr>
	<tr>
		<td colspan='2'><b>- Quais ações abaixo foram contempladas no Projeto Político Pedagógico da escola?</b></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan='2'>
			<input type="checkbox" name="queopcao1" value="t" <?if($queopcao1 == 't') echo 'checked';?>>
			- Promoção de Segurança Alimentar e Promoção de Alimentação Saudável
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan='2'>
			<input type="checkbox" name="queopcao2" value="t" <?if($queopcao2 == 't') echo 'checked';?>>
			- Promoção da Cultura de Paz e Direitos Humanos
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan='2'>
			<input type="checkbox" name="queopcao3" value="t" <?if($queopcao3 == 't') echo 'checked';?>>
			- Saúde e Prevenção nas Escolas (SPE): Direito Sexual e Reprodutivo e Prevenção das DST/aids
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan='2'>
			<input type="checkbox" name="queopcao4" value="t" <?if($queopcao4 == 't') echo 'checked';?>>
			- Saúde e Prevenção nas Escolas (SPE): Prevenção ao uso de Álcool, Tabaco, Crack e outras Drogas
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan='2'>
			<input type="checkbox" name="queopcao5" value="t" <?if($queopcao5 == 't') echo 'checked';?>>
			- Promoção das Práticas Corporais, Atividade Física e Lazer nas Escolas
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan='2'>
			<input type="checkbox" name="queopcao6" value="t" <?if($queopcao6 == 't') echo 'checked';?>>
			- Promoção da Saúde Ambiental e Desenvolvimento Sustentável
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan='2'>
			<input type="checkbox" name="queopcao7" value="t" <?if($queopcao7 == 't') echo 'checked';?>>
			- Promoção da Saúde Mental no Território Escolar
		</td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan='2'>
			<input type="checkbox" name="queopcao8" value="t" <?if($queopcao8 == 't') echo 'checked';?>>
			- Saúde e Prevenção nas Escolas (SPE): Formação de jovens multiplicadores para atuarem entre pares nas temáticas do direito sexual e reprodutivo e prevenção das DST/aids
		</td>
	</tr>
	<tr>
		<td style="background: rgb(168, 168, 168);" colspan='4'>&nbsp;</td>
	</tr>
	<tr>  
		<td class="SubTituloEsquerda">- O PPP foi planejado de forma intersetorial com a participação da atenção básica ?</td>
		<td width="60%">
			<input type="radio" name="queppp" value="t" <?if($queppp == 't') echo 'checked';?>> SIM
			&nbsp;
			<input type="radio" name="queppp" value="f" <?if($queppp == 'f') echo 'checked';?>> NÃO
		</td>
	</tr>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='4'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvarC1" id="btSalvarC1" value="Salvar" onclick="validaFormPse();">
						<?}?>
						&nbsp;
						<input type="button" class="botao" name="btVoltarC1" id="btVoltarC1" value="Voltar" onclick="location.href='pse.php?modulo=principal/monitoraComp2&acao=A';">
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

var d = document.formulario;

function validaFormPse(){
	d.submit();
}

</script>