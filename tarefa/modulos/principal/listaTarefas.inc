<?php
require_once APPRAIZ . "tarefa/classes/Tarefa.class.inc";
require_once APPRAIZ . "tarefa/classes/Solicitante.class.inc";
require_once APPRAIZ . "includes/classes/dateTime.inc";

# Verifica Sessão Ativa
verificaSessao();

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
//$titulo_modulo = "Lista Todas as Tarefas / Atividades";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '' );

unset($_SESSION['arSolicitante']);
unset($_SESSION['tarid']);
unset($_SESSION['_tartarefa']);
unset($_SESSION['dados_tarefa']);
if($_GET['taridExcluir']){
	$obTarefa = new Tarefa();
	$obTarefa->excluir($_GET['taridExcluir']);
	$obTarefa->commit();
	unset($_GET['taridExcluir']);
	unset($obTarefa);
	$db->sucesso("principal/listaTarefas");
	die;
}

if($_SESSION['tarefa']['filtroArvore']){
	extract($_SESSION['tarefa']['filtroArvore']);
} else {
	$filtrosituacao = array(1,2);
	$_SESSION['tarefa']['filtroArvore']['filtrosituacao'] = $filtrosituacao;
	
	if($_SESSION['tarefa']['boPerfilGerente'] == 'S'){
		$filtrounaidsetorresponsavel = $_SESSION['tarefa']['setorUsuarioLogado'];
		$_SESSION['tarefa']['filtroArvore']['filtrounaidsetorresponsavel'] = $filtrounaidsetorresponsavel;
	} else {
		$filtrounaidsetorresponsavel = $_SESSION['tarefa']['setorUsuarioLogado'];
		$_SESSION['tarefa']['filtroArvore']['filtrounaidsetorresponsavel'] = $filtrounaidsetorresponsavel;
		$filtrousucpfresponsavel = $_SESSION['usucpf'];
		$_SESSION['tarefa']['filtroArvore']['filtrousucpfresponsavel'] = $filtrousucpfresponsavel;
	}
		
	
}
?>
<meta http-equiv="refresh" content="300">
<form action="" method="POST" name="formulario" id="formulario">
<input type="hidden" name="tarid" id="tarid" value="" >
<input type="hidden" name="tipo" id="tipo" value="filtroArvore" />
<table align="center" border="0" class="tabela" bgcolor="#f5f5f5" cellpadding="3" cellspacing="1">
	<tr>
		<td valign="bottom" width="150px" align="right">
			Código da Tarefa:
		</td>
		<td>
			<?= campo_texto( 'filtrotarid', 'N', '', 'Código da tarefa', 25, 255, '', '','','','','id="filtrotarid" onkeypress="return somenteNumeros(event);"'); ?>
		</td>
		<td width="300px" rowspan="7" valign="top" align="left">
			<fieldset>
				<legend>Legendas</legend>
				<label>
				<img src="../imagens/anexo.gif" style="vertical-align: middle"> - Anexo <br />
				<img src="../imagens/restricao.png" style="vertical-align: middle"> - Restrição <br />
				<img src="../imagens/botao_de.png" style="vertical-align: middle"> - Dependência Externa				
				</label>
			</fieldset>
		</td>
	</tr>
	<tr>
		<td valign="bottom" width="150px" align="right">
			Título da Tarefa:
		</td>
		<td>
			<?= campo_texto( 'filtrotartitulo', 'N', '', 'Título da tarefa', 60, 255, '', '','','','','id="filtrotartitulo"'); ?>
		</td>
	</tr>
	<tr>
		<td valign="bottom" width="150px" align="right">
			Número SIDOC:
		</td>
		<td>
			<?= campo_texto( 'filtrosidoc', 'N', '', 'Número SIDOC', 25, 255, '', '','','','','id="filtrosidoc"'); ?>
		</td>
	</tr>	
	<tr>
		<td valign="bottom" width="150px" align="right">
			Setor de Origem:
		</td>
		<td>
			<?php
				$sql = "SELECT unaid as codigo, unasigla||' - '|| unadescricao as descricao FROM tarefa.unidade";
				$db->monta_combo( "filtrounaidsetororigem", $sql, 'S', 'Selecione...', '', '', '', '', 'N', 'filtrounaidsetororigem',false,null,'Setor de Origem');
			?>
		</td>
	</tr>
	<tr>
		<td valign="bottom" align="right">
			Setor Responsável:
		</td>
		<td>
			<?php
				$sql = "SELECT unaid as codigo, unasigla||' - '|| unadescricao as descricao FROM tarefa.unidade";
				$db->monta_combo( "filtrounaidsetorresponsavel", $sql, 'S', 'Selecione...', 'filtraSetorResponListaTarefa', '', '', '', 'N', 'filtrounaidsetorresponsavel',false,null,'Setor de Responsável');
			?>
		</td>
	</tr>
	<tr>
		<td valign="bottom" align="right">
			Pessoa Responsável:
		</td>
		<td id="td_usucpfresponsavel2">
			<?php
				if($filtrounaidsetorresponsavel){
					$sql = "select distinct ur.usucpf as codigo, u.usunome as descricao 
								from tarefa.usuarioresponsabilidade ur
	   							inner join seguranca.usuario u on ur.usucpf = u.usucpf
	   							where ur.unaid = {$filtrounaidsetorresponsavel} and ur.rpustatus = 'A' ";
				
					$db->monta_combo('filtrousucpfresponsavel', $sql, $habilitado, "Selecione um Setor Responsável", '', '', '', '405', 'N', 'filtrousucpfresponsavel',false,null,'Responsável pela Tarefa');
				} else {
					$db->monta_combo('filtrousucpfresponsavel', array(), 'S', "Selecione um Setor Responsável", '', '', '', '405', 'N', 'filtrousucpfresponsavel',false,null,'Responsável pela Tarefa');
				}
			?>
		</td>
	</tr>
	<tr>
		<td valign="bottom" width="150px" align="right">
			Prazo de Atendimento:
		</td>
		<td>
			<?php 
			$obData = new Data();
			if($filtroprazoini){
				$filtroprazoini = $obData->formataData($filtroprazoini,"YYYY-mm-dd");
			}
			if($filtroprazofim){
				$filtroprazofim = $obData->formataData($filtroprazofim,"YYYY-mm-dd");
			}
			unset($obData);
			?>
			<?php echo campo_data2( 'filtroprazoini','N', 'S', 'Prazo de Atendimento Inicio', 'S' ); ?>
			&nbsp;até&nbsp;
			<?php echo campo_data2( 'filtroprazofim','N', 'S', 'Prazo de Atendimento Fim', 'S'); ?>
		</td>
	</tr>	
	<tr>
		<td valign="bottom" align="right">
			Situação:
		</td>
		<td>
			<?php
				$sql = "SELECT 
                            sitid AS codigo, 
                            sitdsc AS descricao
                        FROM
                            tarefa.situacaotarefa order by codigo";
				$arSituacao = $db->carregar($sql);
				$filtrosituacao = ($filtrosituacao) ? $filtrosituacao : array();
                                if($arSituacao){
                                    foreach($arSituacao as $situacao){
                                            if(in_array($situacao['codigo'], $filtrosituacao)){
                                                    $ckecked = "checked=\"checked\"";
                                            }
                                            echo "<input type=\"checkbox\" $ckecked name=\"filtrosituacao[]\" value=\"{$situacao['codigo']}\"> ".$situacao['descricao'];
                                            $ckecked = "";					
                                    }
                                }
			?>
		</td>
	</tr>
	<tr>
		<td align="right">
			<input type="button" name="" value="Pesquisar" onclick="filtroPesquisa('','');" /> 
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function excluirTarefaAtividade(tarid){
	if(confirm('Deseja excluir este registro')){
		window.location.href = "/tarefa/tarefa.php?modulo=principal/listaTarefas&acao=A&taridExcluir="+tarid;
		return true;
	} else {
		return false;
	}
}

function filtraSetorResponListaTarefa(obj){
	document.formulario.filtrousucpfresponsavel.disabled = false;
	td 	   = document.getElementById('td_usucpfresponsavel2');
	select = document.getElementsByName('filtrousucpfresponsavel')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	var ajax = new Ajax.Request('ajax.php', {
		        method:     'post',
		        parameters: 'tipo=unaidSetorResp&unaid=' + $('filtrounaidsetorresponsavel').value+'&boFiltro=1',
		        onComplete: function (res)
		        {
					//$('teste').innerHTML = res.responseText;
					td.innerHTML = res.responseText;
					td.style.visibility = 'visible';
		        }
		  });
}
</script>
<br />
<?php
include 'cabecalhoIncludesListaTarefa.inc';
if($_SESSION['tarefa']['filtroArvore']){
	montarArvore(null,false,true);
	echo "<script type=\"text/javascript\">
			filtroPesquisa('','');
		  </script>";
} else {
	montarArvore();
}
?>