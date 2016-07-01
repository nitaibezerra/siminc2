<?php 

unset($_SESSION['proinfantil']['turid']);

if($_REQUEST['requisicao'] == 'excluirTurma'){
	$sql  = "DELETE FROM proinfantil.mdsalunoatendidopbf WHERE turid = {$_REQUEST['turid']};";	
	$sql .= "DELETE FROM proinfantil.fotos WHERE turid = {$_REQUEST['turid']};";
	$sql .= "DELETE FROM proinfantil.turma WHERE turid = {$_REQUEST['turid']};";
	$db->executar($sql);
	
	if($db->commit()){
		$db->sucesso('novasturmas/listaTurmas');
	}

	echo "<script>
			alert('Não foi possível excluir a turma!');
			document.location.href = 'proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A';
		  </script>";
	die;
}

if(!$_SESSION['proinfantil']['muncod']){
	$sql = "SELECT	muncod 
			FROM 	proinfantil.usuarioresponsabilidade 
			WHERE	usucpf = '{$_SESSION['usucpf']}' 
			AND 	pflcod = ".EQUIPE_MUNICIPAL;
	
	$muncod = $db->pegaUm($sql);
	$muncod = $muncod ? $muncod : $_REQUEST['muncod'];
	
	if($muncod){
		$_SESSION['proinfantil']['muncod'] = $muncod;
	}
}

if(empty($_SESSION['proinfantil']['muncod'])){
	echo "<script>
			alert('Erro de variável!');
			document.location.href = 'proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A';
		  </script>";
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once "_funcoes_novasturmas.php";
$perfis = pegaPerfil($_SESSION["usucpf"]);
?>

<br/>
<table class="tabela" align="center" cellspacing="0" cellpadding="3" width="100%">
	<tr class="subtituloEsquerda">
		<td height="10" width="45%">
			<input type="button" value="Voltar" id="btnVoltar" />
		</td>
		<td class="TituloTela">Novas Turmas</td>
	</tr>
</table>
<br/>

<?php
$docid = criaDocumentoNovasTurmas($_SESSION['proinfantil']['muncod']);
$esdid = pegaEstadoAtualNovasTurmas($docid);

if($_SESSION['baselogin']=="simec_desenvolvimento") {
	$abacod_tela = 57524;
	$arMnuid = array(11766, 11767);
}else{
	$abacod_tela = 57524;
	$arMnuid = array(11125);
}

if( /*$esdid != WF_NOVASTURMAS_EM_ANALISE ||*/ in_array(EQUIPE_MUNICIPAL,$perfis) || in_array(SECRETARIO_ESTADUAL,$perfis)){
	$arMnuid[] = 11139;
}

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

monta_titulo('Lista de turmas', 'Filtros');

if(!$_SESSION['proinfantil']['muncod'] && !$db->testa_superuser()){
	
	echo "<script>
			alert('Usuário sem responsabiliade!');
			document.location.href = 'proinfantil.php?modulo=inicio&acao=C';
		  </script>";	
}

if($_POST){
	extract($_POST);
}
?>
<?php cabecalhoTurma(); ?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="JavaScript" src="../includes/calendario.js"></script>
<script type="text/javascript">
	$('#btnVoltar').click(function(){
		document.location.href = 'proinfantil.php?modulo=novasturmas/listaMunicipios&acao=A';
	});
	
	$(function(){
		$('#btn_pesquisar').click(function(){
			$('[name=formulario]').submit();
		});	
	
		$('#btn_limpar').click(function(){
			document.location.href = 'proinfantil.php?modulo=novasturmas/listaTurmas&acao=A';
		});
	});
	
	function alterarTurma(ttuid,tirid,turid){
		switch(ttuid){
			case 1:	tipodependencia = 'creche'; 	break;
			case 2: tipodependencia = 'preescola'; break;
			case 3: tipodependencia = 'unificada'; break;
		}
		
		switch(tirid){
			case 1:	tipo = 'publica'; 	 break;
			case 2: tipo = 'conveniada'; break;
		}

		var turma = tipodependencia + '_' + tipo;
		return window.open('proinfantil.php?modulo=novasturmas/popupFormTurma&acao=A&escolhaTurma='+turma+'&turid='+turid,'modelo',"height=600,width=950,scrollbars=yes,top=50,left=200" );
	}
	
	function fotosTurma(turid){
		return window.open('proinfantil.php?modulo=novasturmas/formFotosTurma&acao=A&turid='+turid,'modelo',"height=600,width=950,scrollbars=yes,top=50,left=200" );
	}
	
	function excluirTurma(turid){
		if(confirm('Deseja excluir a turma?')){
			document.location.href = 'proinfantil.php?modulo=novasturmas/listaTurmas&acao=A&requisicao=excluirTurma&turid='+turid;
		}
	}
</script>
<style>
.listagem{
	width: 100%;
}
</style>
<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
	<tr>
		<td valign="top">
			<form method="post" name="formulario" id="formulario" action="">
				<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 style="width:100%;">
					<tr>
						<td class="subtituloDireita">Código INEP</td>
						<td>
							<?php echo campo_texto('codinep', 'N', 'S', '', 70, '', '', ''); ?>	
						</td>
					</tr>					
					<tr>
						<td class="subtituloDireita">Nome da Turma</td>
						<td>
							<?php echo campo_texto('turdsc', 'N', 'S', '', 70, '', '', ''); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">Nome do Estabelecimento</td>
						<td>
							<?php echo campo_texto('descestabelecimento', 'N', 'S', '', 70, '', '', ''); ?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">&nbsp;</td>
						<td class="subtituloEsquerda">
							<input type="button" value="Pesquisar" id="btn_pesquisar" />
							<input type="button" value="Limpar" id="btn_limpar" />
						</td>
					</tr>
				</table>
			</form>
			<?php
			
			$arWhere = array();
			if($_REQUEST['turdsc']){
				$arWhere[] = "(removeacento(turdsc) ilike '%{$_REQUEST['turdsc']}%' or turdsc ilike '%{$_REQUEST['turdsc']}%')";
			}
			
			if($_REQUEST['turdtinicio'] || $_REQUEST['turdtfim']){
				
				$_REQUEST['turdtinicio'] = $_REQUEST['turdtinicio'] ? formata_data_sql($_REQUEST['turdtinicio']) : '';
				$_REQUEST['turdtfim'] 	 = $_REQUEST['turdtfim'] ? formata_data_sql($_REQUEST['turdtfim']) : '';
				
				if($_REQUEST['turdtinicio'] && $_REQUEST['turdtfim']){
					$arWhere[] = "turdtinicio between '{$_REQUEST['turdtinicio']}' and '{$_REQUEST['turdtfim']}'";
				} elseif($_REQUEST['turdtinicio']){
					$arWhere[] = "turdtinicio = '{$_REQUEST['turdtinicio']}'";
				} elseif($_REQUEST['turdtfim']){
					$arWhere[] = "turdtinicio = '{$_REQUEST['turdtfim']}'";
				}
			}
			
			if($_REQUEST['codinep']){
				$arWhere[] = " ent.entcodent = '{$_REQUEST['codinep']}' ";
			}

			if($_SESSION['proinfantil']['muncod']){
				$arWhere[] = " tur.muncod = '{$_SESSION['proinfantil']['muncod']}' ";
			}
			
			if($_REQUEST['descestabelecimento']){
				$arWhere[] = "(ent.entnome ilike '%{$_REQUEST['descestabelecimento']}%' or turnomeescola ilike '%{$_REQUEST['descestabelecimento']}%' or ent.entcodent = '{$_REQUEST['descestabelecimento']}')";				
			}
			
			if($esdid == WF_NOVASTURMAS_EM_CADASTRAMENTO){
				$stAcoes = "<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarTurma(' || ttuid || ',' || tirid || ',' || tur.turid || ')\" alt=\"Editar\" title=\"Editar\"/>&nbsp;
							<img src=\"../imagens/cam_foto.gif\" style=\"cursor:pointer;\" onclick=\"fotosTurma(' || tur.turid || ')\" />&nbsp;
							<img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" onclick=\"excluirTurma(' || tur.turid || ')\" alt=\"Excluir\" title=\"Excluir\"/>";
			
				$arSelect = ""; 
				$cabecalho = array('Ação', 'Cod. INEP', 'Nome Estabelecimento', 'Nome da turma', 'Data início');
			
			} else {
				$stAcoes = "<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"alterarTurma(' || ttuid || ',' || tirid || ',' || tur.turid || ')\" alt=\"Editar\" title=\"Editar\"/>&nbsp;
							<img src=\"../imagens/cam_foto.gif\" style=\"cursor:pointer;\" onclick=\"fotosTurma(' || tur.turid || ')\" />";
				$arSelect = ", CASE WHEN ana.anaparecer IS NULL OR ana.anaparecer = '' THEN ' - ' ELSE ana.anaparecer END, 
						   CASE WHEN ana.anatipo = 'A' THEN 'Aprovada' WHEN ana.anatipo = 'I' THEN 'Indeferida' WHEN ana.anatipo = 'D' THEN 'Diligenciada' ELSE ' - ' END as status";
				$cabecalho = array('Ação', 'Cod. INEP', 'Nome Estabelecimento', 'Nome da turma', 'Data início', 'Parecer','Status');
			}
			
			$sql = "SELECT '{$stAcoes}' as acao,
								ent.entcodent,
								CASE WHEN entnome IS NULL THEN turnomeescola ELSE entnome END AS estabelecimento,
								turdsc,									
								to_char(turdtinicio, 'dd/MM/yyyy')
								$arSelect
					FROM		proinfantil.turma tur
					LEFT JOIN	entidade.entidade ent ON ent.entcodent = tur.entcodent
					LEFT JOIN 	proinfantil.analisenovasturmasaprovacao ana ON ana.turid = tur.turid
					inner join proinfantil.procenso p on /*p.entcodent = tur.entcodent and*/ p.muncod = tur.muncod 
					WHERE 
						p.prcano = '2012'".
						($arWhere ? ' AND '.implode(' AND ', $arWhere) : '')
						." AND tur.turstatus = 'A'
						AND tur.turano <= '2012'
					group by ent.entcodent, entnome, turnomeescola, turdsc, turdtinicio, tur.ttuid, tur.tirid, tur.turid, ana.anaparecer, ana.anatipo
					ORDER BY 	tur.turdsc";
			
			$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '', '', array(80, 80, null, null, 180));
		?>
		</td>
		<?php if($docid): ?>
		<td width="100" valign="top" align="center">				
			<?php
			if( $_SESSION['exercicio'] == date('Y') && $esdid != WF_NOVASTURMAS_EM_DILIGENCIA ){
				$arrOcultar = array('acoes' => true, 'historico'=>false);
			} else {
				$arrOcultar = array('acoes' => false, 'historico'=>false);
			}
			
			wf_desenhaBarraNavegacao( $docid , array('muncod' => $_SESSION['proinfantil']['muncod']),  $arrOcultar );
			?>				
		</td>
		<?php endif; ?>
	</tr>
</table>