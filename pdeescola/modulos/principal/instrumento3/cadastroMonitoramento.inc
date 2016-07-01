<?php
pde_verificaSessao();
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
if(isset($_SESSION["pdeid"]) && isset($_GET["qapid"])) {
	$pdeid = $_SESSION["pdeid"];
	$qapid = $_GET["qapid"];
}else{
	echo "<script language=\javascript\" type=\"text/javascript\">
			alert('Erro no envio dos parâmetros necessários.');
			window.history.go(-1);
		  </script>";
	exit;
}

$cDado = array("instrumento" => 3);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Monitoramento', '');
echo cabecalhoPDE();

function salva (){
	global $db,$pdeid,$qapid;
	
	if($_POST["submetido"]) {
		$qapid = $_POST["qapid"];
		$pdeid = $_POST["pdeid"];
		
		$updateresposta = $db->pegaUm("SELECT 
												aplid
											 FROM 
											 	pdeescola.avaliacaoplano
											 WHERE
											 	pdeid = $pdeid
											 AND
											 	qapid = $qapid");
		if($updateresposta) {
			$db->executar("UPDATE 
							pdeescola.avaliacaoplano 
						   SET 
						   	eavid = ".$_POST["escala_marcada"].", aplsugestao = '".substr($_POST["sugestao"], 0, 150 )."' 
						   WHERE
						   	qapid = ".$qapid." AND pdeid = ".$pdeid);

		}else {
			$db->executar("INSERT INTO 
							pdeescola.avaliacaoplano(qapid,pdeid,eavid,aplsugestao)						
						   VALUES(".$qapid.",".$pdeid.",".$_POST["escala_marcada"].",'".substr($_POST["sugestao"], 0, 150 )."')");
			}
			
		
	
		$ppritem = 'cadastroMonitoramento_'.$qapid;
		
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
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'pm', '$ppritem','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p );
		}
		$db->commit();
		
	}
}

// Recupera os dados da pergunta.
$dados = $db->carregar("SELECT
						  aplid as id, eavid as nota, aplsugestao as sugestao
						FROM
						  pdeescola.avaliacaoplano
						WHERE
						  pdeid = ".$pdeid." AND qapid = ".$qapid);

$tipo = ($dados[0]["id"] == NULL) ? 'insert' : 'update';
$nota = ($dados[0]["nota"] == NULL) ? '' : $dados[0]["nota"];
$sugestao = ($dados[0]["sugestao"] == NULL) ? '' : $dados[0]["sugestao"];

// Recupera as categorias 'pai' da pergunta.
$pergunta = $db->carregar("SELECT
							qap.qapcodigo as cod_filho, 
							qap.qapdescricao as desc_filho,  
							qap2.qapcodigo as cod_pai, qap2.qapdescricao as desc_pai,
							qap3.qapcodigo as cod_avo, qap3.qapdescricao as desc_avo
						   FROM
						   	pdeescola.questaoavaliacaoplano qap
						   INNER JOIN
						   	pdeescola.questaoavaliacaoplano qap2 ON qap2.qapid = qap.qapidpai
						   INNER JOIN
						   	pdeescola.questaoavaliacaoplano qap3 ON qap3.qapid = qap2.qapidpai
						   WHERE
						   	qap.qapid = ".$qapid." AND qap.qapano = ".ANO_EXERCICIO_PDE_ESCOLA);

//Recupera o campo 'qapid' da pergunta anterior e da próxima.
$sequencial = $pergunta[0]["sequencial_filho"];

$anterior = $db->pegaUm("SELECT qapid FROM pdeescola.questaoavaliacaoplano WHERE qapano = ".ANO_EXERCICIO_PDE_ESCOLA);
$anterior = ($anterior == NULL) ? '' : $anterior;

$proximo = $db->pegaUm("SELECT qapid FROM pdeescola.questaoavaliacaoplano WHERE qapano = ".ANO_EXERCICIO_PDE_ESCOLA);
$proximo = ($proximo == NULL) ? '' : $proximo;
?>
<br />
<?=subCabecalho('Avaliação Geral do Programa/ Autoavaliação '); ?>	
<form method="post" id="form_monitoramento" name="form_monitoramento">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="qapid" value="<?=$qapid?>">
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
			<b><?=$pergunta[0]["cod_filho"]?>&nbsp;<?=$pergunta[0]["desc_filho"]?></b>
			<br /><br />
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Pontuação:</td>
		<td>
			<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" width="30%">
				<?php
					$escala = $db->carregar("SELECT 
												eavid as codigo, eavdescricao as descricao 
											 FROM 
											 	pdeescola.escalaavaliacao
											 ORDER BY
											 	eavid ASC");
					
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
		<td class="SubTituloDireita">Comentário/Sugestão:</td>
		<td>
			<?=campo_textarea('sugestao',false,'S','', 100, 5, 150,'id="sugestao"');?>
		</td>
	</tr>
	<? $cForm->montarbuttons("submeteDadosPergunta()",$dir='propostaMonitoramento');?>				
</table>
</form>

<script language="javascript" type="text/javascript">

function retorna() {
	window.location = "?modulo=principal/propostaMonitoramento&acao=A";
}

function marcaEscala(cod) {
	document.getElementById('escala_marcada').value = cod;
}

function submeteDadosPergunta() {

	if( document.getElementById('sugestao').value.length > 150 )
	{
		alert("Você excedeu o nº máximo de caracteres (150) do campo 'Comentário/Sugestão'");
		return false;
	}
	
	if(document.getElementById('escala_marcada').value == '')
	{
		alert("A 'Pontuação' deve ser informada.");
		return false;
//	}
//	else if(document.getElementById('sugestao').value == '')
//	{
//		alert("O campo 'Comentário/Sugestão' deve ser informado.");
//		return false;
	}
	else
	{
		document.getElementById('form_monitoramento').submit();
		return true;
	}
	
}

function perguntaAnterior() {
	window.location = "?modulo=principal/instrumento3/cadastroMonitoramento&acao=A&pdeid=<?=$pdeid?>&qapid=<?=$anterior?>";
}

function proximaPergunta() {
	window.location = "?modulo=principal/instrumento3/cadastroMonitoramento&acao=A&pdeid=<?=$pdeid?>&qapid=<?=$proximo?>";
}

</script>