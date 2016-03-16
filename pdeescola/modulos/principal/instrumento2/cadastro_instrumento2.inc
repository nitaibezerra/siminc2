<?php
pde_verificaSessao();
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if(isset($_SESSION["pdeid"]) && isset($_GET["aceid"])) {
	$pdeid = $_SESSION["pdeid"];
	$aceid = $_GET["aceid"];
}else{
	echo "<script language=\javascript\" type=\"text/javascript\">
			alert('Erro no envio dos parâmetros necessários.');
			window.history.go(-1);
		  </script>";
	exit;
}

$cDado = array("instrumento" => 2);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Instrumento 2', '');
echo cabecalhoPDE();

function salva (){
	global $db,$pdeid,$aceid;
	
	if($_POST["submetido"]) {
		$aceid = $_POST["aceid"];
		$pdeid = $_POST["pdeid"];
		
		$updateresposta = $db->pegaUm("SELECT 
												dacid
											 FROM 
											 	pdeescola.detalheanalisecriterioeficacia
											 WHERE
											 	pdeid = $pdeid
											 AND
											 	aceid = $aceid");
		
		if($updateresposta) {
			$db->executar("UPDATE 
							pdeescola.detalheanalisecriterioeficacia 
						   SET 
						   	esaid = ".$_POST["escala_marcada"].", dacevidencia = '".substr($_POST["evidencia"], 0, 500 )."' 
						   WHERE
						   	aceid = ".$aceid." AND pdeid = ".$pdeid);
			}
		else {
			$db->executar("INSERT INTO 
							pdeescola.detalheanalisecriterioeficacia(aceid,pdeid,esaid,dacevidencia)						
						   VALUES(".$aceid.",".$pdeid.",".$_POST["escala_marcada"].",'".substr($_POST["evidencia"], 0, 500 )."')");
		}
		
		
	
		$ppritem = 'cadastro_instrumento2_'.$aceid;
		
		$existe_preenchimento = $db->pegaUm("SELECT 
												pprid
											 FROM 
											 	pdeescola.pdepreenchimento
											 WHERE
											 	pdeid = '$pdeid'
											 AND
											 	ppritem = '$ppritem'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i2', '$ppritem','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p );
		}
		$db->commit();
		
	}
}

// Recupera os dados da pergunta.
$dados = $db->carregar("SELECT
						  dacid as id, esaid as nota, dacevidencia as evidencia
						FROM
						  pdeescola.detalheanalisecriterioeficacia
						WHERE
						  pdeid = ".$pdeid." AND aceid = ".$aceid);

$tipo = ($dados[0]["id"] == NULL) ? 'insert' : 'update';
$nota = ($dados[0]["nota"] == NULL) ? '' : $dados[0]["nota"];
$evidencia = ($dados[0]["evidencia"] == NULL) ? '' : $dados[0]["evidencia"];

// Recupera as categorias 'pai' da pergunta.
$pergunta = $db->carregar("SELECT
							ace.acecodigo as cod_filho, 
							ace.acedescricao as desc_filho,
							ace.acedescricaopr as desc_filhopr,
							ace.aceseq as sequencial_filho,  
							ace2.acecodigo as cod_pai, ace2.acedescricao as desc_pai,
							ace3.acecodigo as cod_avo, ace3.acedescricao as desc_avo
						   FROM
						   	pdeescola.analisecriterioeficacia ace
						   INNER JOIN
						   	pdeescola.analisecriterioeficacia ace2 ON ace2.aceid = ace.aceidpai
						   INNER JOIN
						   	pdeescola.analisecriterioeficacia ace3 ON ace3.aceid = ace2.aceidpai
						   WHERE
						   	ace.aceid = ".$aceid." AND ace.aceano = ".ANO_EXERCICIO_PDE_ESCOLA);

//Recupera o campo 'aceid' da pergunta anterior e da próxima.
$sequencial = $pergunta[0]["sequencial_filho"];

$anterior = $db->pegaUm("SELECT aceid FROM pdeescola.analisecriterioeficacia WHERE aceseq = ".($sequencial - 1)." AND aceano = ".ANO_EXERCICIO_PDE_ESCOLA);
$anterior = ($anterior == NULL) ? '' : $anterior;

$proximo = $db->pegaUm("SELECT aceid FROM pdeescola.analisecriterioeficacia WHERE aceseq = ".($sequencial + 1)." AND aceano = ".ANO_EXERCICIO_PDE_ESCOLA);
$proximo = ($proximo == NULL) ? '' : $proximo;
?>
<br />
<?=subCabecalho('Análise dos Critérios de Eficácia Escolar'); ?>	
<form method="post" id="form_instr2" name="form_instr2">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="aceid" value="<?=$aceid?>">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<input type="hidden" name="tipo" value="<?=$tipo?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Pergunta:</td>
		<td>
			<b><?=$pergunta[0]["cod_avo"]?>&nbsp;<?=$pergunta[0]["desc_avo"]?></b><br />
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<img src="../imagens/seta_filho.gif">
			<b><?=$pergunta[0]["cod_pai"]?>&nbsp;<?=$pergunta[0]["desc_pai"]?></b><br />
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			<?if($_SESSION['estado'] == 'PR') {?>
			<img src="../imagens/seta_filho.gif">
			<b><?=$pergunta[0]["cod_filho"]?>&nbsp;<?=$pergunta[0]["desc_filhopr"]?></b>
			<?}else{ ?>
			<b><?=$pergunta[0]["cod_filho"]?>&nbsp;<?=$pergunta[0]["desc_filho"]?></b>
			<?} ?>
			<br /><br />
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Escala:</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="30%">
				<?php
					$escala = $db->carregar("SELECT 
												esaid as codigo, esadescricao as descricao 
											 FROM 
											 	pdeescola.escalaace
											 ORDER BY
											 	esaid DESC");
					
					for($i=0; $i<count($escala); $i++) {
				?>			
					<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
						<td align="center">
							<input type="radio" name="escala" value="<?=$escala[$i]["codigo"]?>" 
							onclick="marcaEscala('<?=$escala[$i]["codigo"]?>');" 
							<?php if($escala[$i]["codigo"] == $nota) echo "checked"; ?> />
						</td>
						<td>								
							&nbsp;<?=$escala[$i]["descricao"]?>								
						</td>	
					</tr>	
				<? } ?>
			</table>
		</td>
		<input type="hidden" id="escala_marcada" name="escala_marcada" value="<?=$nota?>">
	</tr>
	<tr>
		<td class="SubTituloDireita">Evidências:</td>
		<td>
			<?=campo_textarea('evidencia',false,'S','', 100, 5, 500,'id="evidencia"');?>
		</td>
	</tr>
	<? $cForm->montarbuttons("submeteDadosPergunta()");?>				
</table>
</form>

<script language="javascript" type="text/javascript">

function retorna() {
	window.location = "?modulo=principal/estrutura_avaliacao&acao=A";
}

function marcaEscala(cod) {
	document.getElementById('escala_marcada').value = cod;
}

function submeteDadosPergunta() {

	if( document.getElementById('evidencia').value.length > 500 )
	{
		alert("Você excedeu o nº máximo de caracteres (500) do campo 'Evidência");
		return false;
	}
	
	if(document.getElementById('escala_marcada').value == '')
	{
		alert("A escala deve ser informada.");
		return false;
	}
	else if(document.getElementById('evidencia').value == '')
	{
		alert("O campo 'Evidências' deve ser informado.");
		return false;
	}
	else
	{
		document.getElementById('form_instr2').submit();
		return true;
	}
	
}

function perguntaAnterior() {
	window.location = "?modulo=principal/instrumento2/cadastro_instrumento2&acao=A&pdeid=<?=$pdeid?>&aceid=<?=$anterior?>";
}

function proximaPergunta() {
	window.location = "?modulo=principal/instrumento2/cadastro_instrumento2&acao=A&pdeid=<?=$pdeid?>&aceid=<?=$proximo?>";
}

</script>