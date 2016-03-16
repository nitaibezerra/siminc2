<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$where = array();

$responsabilidade = retornaResponsabilidades($_SESSION['usucpf'], 'U', 'cod');

/**
 * Bloco que valida as diferentes ações, como a inclusão ou download de arquivo.
 */
switch ($_REQUEST['request']){
	case 'anexa_arquivo':
		//ver($_REQUEST['ptastatusfinal'], d);
		// -- Verifica se tem algum arquivo para salvar no servidor
		if ($_FILES['file']['size'] > 0) {
				
			$nomeArquivo = $_REQUEST['aiddsc'];

			$ptastatusfinal = $_REQUEST['ptastatusfinal'] != '' ? 't' : 'f';
			
			$descricao = explode(".", $_FILES['file']['name']);
			
			if ($_REQUEST['sis'] == 'pnld'){
				$campos = array(
						"unvid" => "'" . $_GET['unvid'] . "'",
						"ptaano" => "'2016'",
						"ptadtinclusao" => "'" . date('Y-m-d H:i:s') . "'",
						"usucpf" => "'" . $_REQUEST['usucpf'] . "'",
						"ptastatusfinal" => "'" . $ptastatusfinal . "'",
						"ptaindicadormecuniv" => "'" . $_REQUEST['ptaInd'] . "'"
				);
				
				$tabela = 'ptapnld';
			} else {
				$campos = array(
						"unvid" => "'" . $_GET['unvid'] . "'",
						"pnbano" => "'2015'",
						"pnbdtinclusao" => "'" . date('Y-m-d H:i:s') . "'",
						"usucpf" => "'" . $_REQUEST['usucpf'] . "'",
						"pnbstatusfinal" => "'" . $ptastatusfinal . "'",
						"pnbindicadormecuniv" => "'" . $_REQUEST['ptaInd'] . "'"
				);
				$tabela = 'ptapnbe';
			}
		
			$file = new FilesSimec($tabela, $campos, "livro");
			$arquivoSalvo = $file->setUpload($_FILES ['file']['name'], '', true);
			if ($arquivoSalvo) {
				echo "
						<script>
							alert('Arquivo cadastrado com sucesso!');
                            window.opener.location.href='livro.php?modulo=principal/pnld_documentos_2016&sis=".$_REQUEST['sis']."&acao=A';
                            self.close();
						</script>
					";
			}
		}
		
		break;
		
	case 'download_arquivo':
		$file = new FilesSimec();
		if ($_REQUEST['arqid']) {
			ob_clean();
			$arquivo = $file->getDownloadArquivo($_REQUEST['arqid']);
			
			if ($_GET['unvid']){
				echo "
							<script>
								window.location.href = 'livro.php?modulo=principal/pnld_documentos_2016&sis=".$_REQUEST['sis']."&acao=A&tipo=C&unvid=".$_GET['unvid']."';
							</script>";
			} else {
				echo "
							<script>
								window.location.href = 'livro.php?modulo=principal/pnld_documentos_2016&sis=".$_REQUEST['sis']."&acao=A';
							</script>";
			}
		}
		
		break;
		
	case 'excluir_arquivo':
		$campos = array(
						"arqid" => "'" . $_REQUEST['arqid'] . "'"
				);
		$tabela = 'ptapnld';
				
		$file = new FilesSimec($tabela, $campos, "livro");
		
		if ($_REQUEST['arqid']) {
			
			$arquivoDeletado = $file->setRemoveUpload( $_REQUEST['arqid'] );
			
			if($arquivoDeletado){
				if ($_GET['unvid']){
					echo "
								<script>
									alert('Arquivo apagado com sucesso');
									window.location.href = 'livro.php?modulo=principal/pnld_documentos_2016&sis=".$_REQUEST['sis']."&acao=A&tipo=C&unvid=".$_GET['unvid']."';
								</script>";
				} else {
					echo "
								<script>
									alert('Arquivo apagado com sucesso');
									window.location.href = 'livro.php?modulo=principal/pnld_documentos_2016&sis=".$_REQUEST['sis']."&acao=A';
								</script>";
				}
			}
		}
		
		break;
}

$arrPerfil = arrayPerfil();

/**
 * Determina o tipo de inclusão de arquivo:
 * 		- 'm' sifnifica que o arquivo foi incluído por um usuário do MEC;
 *		- 'u' Signfica que o arquivo foi incluído por um usuário da universidade e, por isso, 
 *		pracisa ser limitado as universidades vinculados à ele.
 */
if ( in_array(PNLD_PERFIL_GESTOR_PTA, $arrPerfil) ||
in_array(PNLD_SUPER_USER, $arrPerfil) ){
	$ptaInd = 'm';
} else {
	$ptaInd = 'u';
	$where[] = " AND ur.unvid in ( " . implode(', ', $responsabilidade) . " )";
}

/**
 * Bloco de código que apresenta os formulários, de acordo com o tipo.
 * 		- Tipo A retorna o formulário de inclusão de arquivo;
 * 		- Tipo C retorna a lista de arquivos da universidade;
 */
if ($_GET['unvid']){
	
	$sql = "SELECT
						unv.univdsc,
						CASE
							WHEN com.comdsc IS NOT NULL THEN com.comdsc
							ELSE '-'
						END AS comdsc
					FROM livro.ptauniversidade unv
					LEFT JOIN livro.componente com ON unv.comid = com.comid
					WHERE unv.unvid = " . $_GET['unvid'];
		
	$res = $db->carregar($sql);
	$res = $res[0];
	
	switch ($_GET['tipo']){
		case 'A':
			monta_titulo('Anexar Arquivo', $linha2);
						
			?>
			    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
			    <link rel="stylesheet" type="text/css" href="../includes/layout/pdeinterativo/css/simec.css"/>
			    
			    <script type="text/javascript" src="../../includes/funcoes.js"></script>
			    <script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
				
				<script type="text/javascript">
					function anexarArquivo(usucpf, ptaindicador){
						var formulario = document.formulario;
			
						formulario.request.value = 'anexa_arquivo';
						formulario.usucpf.value = usucpf;
						formulario.ptaInd.value = ptaindicador;
			
						formulario.submit();
					}
				</script>
				
				<form name="formulario" id="formulario" enctype="multipart/form-data" action="" method="post">
					<input type="hidden" name="request" id="request" value="" />
					<input type="hidden" name="ptaInd" id="ptaInd" value="" />
					<input type="hidden" name="usucpf" id="usucpf" value="" />
					
					<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
						<tr>
							<td align="right" class="SubTituloDireita" style="width:100px;">Universidade:</td>
							<td>
								<input type="text" id="unvdsc" name="unvdsc" disabled style="width: 350px;" value="<?php echo $res['univdsc'] ?>" />
							</td>
						</tr>
						<tr>
							<td align="right" class="SubTituloDireita" style="width:100px;">Disciplina:</td>
							<td>
								<input type="text" id="comdsc" name="comdsc" disabled style="width: 350px;" value="<?php echo $res['comdsc'] ?>" />
							</td>
						</tr>
					</table>
					
				    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
						<tr>
							<td align="right" class="SubTituloDireita" style="width:100px;">Arquivo:</td>
							<td>
								<input type="hidden" name="arqid" id="arqid" >
								<input type="hidden" name="aidid" id="aidid" >
								<input type="file" class="btn btn-primary start" name="file" id="file" title="Selecionar arquivo" />
							</td>
						</tr>
						<?php if ($ptaInd == 'm') { ?>
						<tr>
							<td align="right" class="SubTituloDireita" style="width:100px;">Versão final:</td>
							<td>
								<input type="checkbox" id="ptastatusfinal" name="ptastatusfinal" />
							</td>
						</tr>
						<?php } ?>
				        <tr>
				            <td align="center" colspan="2">
				                <input type="button" value="Salvar" id="btSalvar" onclick="anexarArquivo('<?php echo $_SESSION['usucpf'] ?>', '<?php echo $ptaInd ?>')">
				            </td>
				        </tr>
					</table>
				</form>
			<?php 
			break;
			
		case 'C':
			monta_titulo('Lista de versões PTA', $linha2);
				
			?>
		    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		    <link rel="stylesheet" type="text/css" href="../includes/layout/pdeinterativo/css/simec.css"/>
		    
		    <script type="text/javascript" src="../../includes/funcoes.js"></script>
		    <script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
			
			<script type="text/javascript">
				function downloadPta(arqid){
					var formulario = document.formulario;

					formulario.request.value = 'download_arquivo';
					formulario.arqid.value = arqid;

					formulario.submit();
				}
				
				function excluirPta(arqid){
				
					if(confirm('Deseja realmente apagar este anexo?')){
				
						var formulario = document.formulario;
	
						formulario.request.value = 'excluir_arquivo';
						formulario.arqid.value = arqid;
	
						formulario.submit();
					}
				}
			</script>
			
			<form name="formulario" id="formulario" action="" method="post">
				<input type="hidden" name="request" id="request" value="" />
				<input type="hidden" name="arqid" id="arqid" value="" />
				
				<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
					<tr>
						<td align="right" class="SubTituloDireita" style="width:100px;">Universidade:</td>
						<td>
							<input type="text" id="unvdsc" name="unvdsc" disabled style="width: 350px;" value="<?php echo $res['univdsc'] ?>" />
						</td>
					</tr>
					<tr>
						<td align="right" class="SubTituloDireita" style="width:100px;">Disciplina:</td>
						<td>
							<input type="text" id="comdsc" name="comdsc" disabled style="width: 350px;" value="<?php echo $res['comdsc'] ?>" />
						</td>
					</tr>
				</table>
				
				
				<?php
		
				$btnDownload   = "<img style=\'cursor:hand\' title=\'Baixar arquivo\' src=\'../imagens/anexo.gif\' onclick=\'downloadPta('||pta.arqid||')\'>";
				
				if ( in_array(PNLD_PERFIL_GESTOR, $arrPerfil) || in_array(PNLD_SUPER_USER, $arrPerfil) ){
					$btnExcluirAnexo   = "<img style=\'cursor:hand\' title=\'Apagar arquivo\' src=\'../imagens/excluir.gif\' onclick=\'excluirPta('||pta.arqid||')\'>";
				}
				
				if ($_REQUEST['sis'] == 'pnld') {
					$sql = "SELECT
								'<center> $btnDownload $btnExcluirAnexo </center>' AS acao,
								usu.usunome,
								pta.ptaano,
								to_char(pta.ptadtinclusao, 'dd/mm/yyyy') as ptadtinclusao,
								to_char(pta.ptadtinclusao, 'HH24:ii') as ptahrinclusao,
								CASE
									WHEN pta.ptaindicadormecuniv = 'm' THEN 'MEC'
									WHEN pta.ptaindicadormecuniv = 'u' THEN 'Universidade'
								END AS ptaindicadormecuniv,
								CASE
									WHEN ptastatusfinal = 'f' THEN 'Não'
									WHEN ptastatusfinal = 't' THEN 'Sim'
								END AS ptastatusfinal
							FROM livro.ptapnld pta
							INNER JOIN seguranca.usuario usu ON usu.usucpf = pta.usucpf
							WHERE pta.unvid = " . $_GET['unvid'];
				} else {
					$sql = "SELECT
								'<center> $btnDownload </center>' AS acao,
								usu.usunome,
								pta.pnbano,
								to_char(pta.pnbdtinclusao, 'dd/mm/yyyy') as ptadtinclusao,
								to_char(pta.pnbdtinclusao, 'HH24:ii') as ptahrinclusao,
								CASE
									WHEN pta.pnbindicadormecuniv = 'm' THEN 'MEC'
									WHEN pta.pnbindicadormecuniv = 'u' THEN 'Universidade'
								END AS ptaindicadormecuniv,
								CASE
									WHEN pnbstatusfinal = 'f' THEN 'Não'
									WHEN pnbstatusfinal = 't' THEN 'Sim'
								END AS ptastatusfinal
							FROM livro.ptapnbe pta
							INNER JOIN seguranca.usuario usu ON usu.usucpf = pta.usucpf
							WHERE pta.unvid = " . $_GET['unvid'];
				}
			
				$cabecalho = Array('Ação', 'Nome', 'Ano', 'Data de Inclusão', 'Hora de Inclusão', 'Origem', 'Versão Final');
				$alinhamento = Array('center', 'left', 'center', 'center', 'center', 'center', 'center');
				$tamanho = Array('5%', '60%', '5%', '10%', '5%', '5%', '5%');
				$db->monta_lista($sql, $cabecalho, 50, 20, 'N', 'left', 'N', '', $tamanho, $alinhamento, '');
				
				?>
			</form>
			<?php 
						
			break;
	}
	
	exit();
}

/**
 * Formulário principal, apresentado na seleção da página no menu 'Principal'.
 */

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

$db->cria_aba($abacod_tela, $url, $parametros);

$ano = $_REQUEST['sis'] == 'pnld' ? ' 2016' : ' 2015';

monta_titulo('PTA '.strtoupper($_REQUEST['sis']). $ano, $linha2);

if ($_POST['request']) {
	if ($_POST['request'] == 'pesq'){
		if ($_POST['unvid'][0]){
			$where[] = " AND unv.unvid in ( " . implode(', ', $_POST['unvid']) . " )";
		}
		
		if ($_POST['comid'][0]){
			$where[] = " AND unv.comid in ( " . implode(', ', $_POST['comid']) . " )";
		}
	}
}

?>

<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />

<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/lib/jquery.cookie.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview-simec.js"></script>

<script type="text/javascript">
	function onOffCampo(campo) {
	    var div_on = document.getElementById(campo + '_campo_on');
	    var div_off = document.getElementById(campo + '_campo_off');
	    var input = document.getElementById(campo + '_campo_flag');
	    if (div_on.style.display == 'none') {
	        div_on.style.display = 'block';
	        div_off.style.display = 'none';
	        input.value = '1';
	    } else {
	        div_on.style.display = 'none';
	        div_off.style.display = 'block';
	        input.value = '0';
	    }
	}

	function pesquisa(tipo){
        var formulario = document.formulario;
        formulario.request.value = tipo;
        console.log(tipo);
        if (tipo == 'pesq') {
            selectAllOptions(formulario.unvid);
            selectAllOptions(formulario.comid);
        } 
        
        formulario.submit();
	}

	function baixarArquivo(arqid){
		var formulario = document.formulario;

		formulario.request.value = 'download_arquivo';
		formulario.arqid.value = arqid;

		formulario.submit();
	}
	
	function anexarPta(unvid){
		window.open("livro.php?modulo=principal/pnld_documentos_2016&sis=<?php echo $_REQUEST['sis'] ?>&acao=A&tipo=A&unvid="+unvid,"pop","toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,left=600,top=200,width=530,height=300");
	}

	function cunsultaPta(unvid){
		window.open("livro.php?modulo=principal/pnld_documentos_2016&sis=<?php echo $_REQUEST['sis'] ?>&acao=A&tipo=C&unvid="+unvid,"pop","toolbar=no,location=no,status=no,menubar=no,scrollbars=yes,resizable=no,left=300,top=200,width=730,height=310");
	}
</script>

<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="request" id="request" value="" />
	<input type="hidden" name="arqid" id="arqid" value="" />
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td>
				<?php 
				
				if ( in_array(PNLD_PERFIL_UNIVERSIDADE_PTA, $arrPerfil) ) {
					$whereR = " AND unv.unvid in ( " . implode(', ', $responsabilidade) . " )";
				}
				
				if ($_REQUEST['sis'] == 'pnbe'){
					$whereR = " AND unv.comid IS NULL";
				} else {
					$whereR = " AND unv.comid IS NOT NULL";
				}
				
				$stSql = "
							SELECT
								unv.unvid as codigo,
								unv.univdsc as descricao
							FROM livro.ptauniversidade unv
							INNER JOIN livro.componente com ON unv.comid = com.comid
							WHERE com.comstatus = 'A'
							{$whereR}
							ORDER BY unv.univdsc";
				
				$stSqlCarregados = "";
				
				if (is_array($_POST['unvid']) && $_POST['unvid'][0]) {
					$stSqlCarregados = "
							SELECT
								unv.unvid as codigo,
								unv.univdsc as descricao
							FROM livro.ptauniversidade unv
							INNER JOIN livro.componente com ON unv.comid = com.comid
							WHERE com.comstatus = 'A'
						    AND unv.unvid IN (" . implode(", ", $_POST['unvid']) . ")
							ORDER BY unv.univdsc";
				}
				mostrarComboPopup('Universidade', 'unvid', $stSql, $stSqlCarregados, 'Selecione a Universidade');
				
				if ($_REQUEST['sis'] != 'pnbe'){
					if ( in_array(PNLD_PERFIL_UNIVERSIDADE_PTA, $arrPerfil) ) {
						$whereRs = " AND comid in ( SELECT comid FROM livro.ptauniversidade WHERE unvid IN ( " . implode(', ', $responsabilidade) . " ))";
					}
					
					$stSql = "
								SELECT
									comid as codigo,
									comdsc as descricao
								FROM livro.componente
								WHERE comstatus = 'A'
								{$whereRs}
								ORDER BY comdsc";
					
					$stSqlCarregados = "";
					
					if (is_array($_POST['comid']) && $_POST['comid'][0]) {
						$stSqlCarregados = "
								SELECT
									comid as codigo,
									comdsc as descricao
								FROM livro.componente
	                            WHERE comstatus = 'A'
								AND comid IN (" . implode(", ", $_POST['comid']) . ")
								ORDER BY comdsc";
					}
					mostrarComboPopup('Componente', 'comid', $stSql, $stSqlCarregados, 'Selecione o Componente');
				}
				?>			
			</td>
		</tr>
        <tr>
            <td align="center" colspan="2">
                <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisa('pesq')"/>
                &nbsp;&nbsp;
                <input type="button" name="Ver Todos" value="Ver Todos" onclick="pesquisa('todo')"/>
            </td>
        </tr>
	</table>
</form>

<?php 
	$btnAnexar   = "<img style=\'cursor:hand\' title=\'Adicionar arquivo\' src=\'../imagens/incluir_p.gif\' onclick=\'anexarPta('||unv.unvid||')\'>";
	$btnConsulta = "<img style=\'cursor:hand\' title=\'Consultar versões\' src=\'../imagens/consultar.gif\' onclick=\'cunsultaPta('||unv.unvid||')\'>";
	
	if ($_REQUEST['sis'] == 'pnld') {

		$lnkAno = "'<a href=\"javascript:baixarArquivo('||pta.arqid||')\" title=\"Baixar arquivo\">'||to_char(ptadtinclusao, 'dd/mm/yyyy')||'</a>'";
		$sql = "SELECT distinct
					CASE
						WHEN ptaf.ptaid is null THEN '<center> $btnAnexar $btnConsulta </center>'
						ELSE '<center> $btnConsulta </center>'
					END as acao,
					unv.unvsigla,
					com.comdsc,
					(SELECT $lnkAno FROM livro.ptapnld pta WHERE pta.unvid = unv.unvid AND pta.ptastatusfinal != 't' AND pta.ptaindicadormecuniv = 'u' ORDER BY pta.ptaid DESC LIMIT 1) as verunv,
					(SELECT $lnkAno FROM livro.ptapnld pta WHERE pta.unvid = unv.unvid AND pta.ptastatusfinal != 't' AND pta.ptaindicadormecuniv = 'm' ORDER BY pta.ptaid DESC LIMIT 1) as vermec,
					(SELECT $lnkAno FROM livro.ptapnld pta WHERE pta.unvid = unv.unvid AND pta.ptastatusfinal = 't' ORDER BY pta.ptaid DESC LIMIT 1) as verfinal
				FROM livro.ptauniversidade unv
				INNER JOIN livro.componente com ON unv.comid = com.comid
				LEFT JOIN livro.ptapnld ptaf ON unv.unvid = ptaf.unvid AND ptaf.ptastatusfinal = 't'
				LEFT JOIN livro.usuarioresponsabilidade ur ON unv.unvid = ur.unvid
				WHERE com.comstatus = 'A'
				" . implode(" ", $where) .
				" ORDER BY unv.unvsigla ";
	} else {
	
		$lnkAno = "'<a href=\"javascript:baixarArquivo('||pta.arqid||')\" title=\"Baixar arquivo\">'||to_char(pnbdtinclusao, 'dd/mm/yyyy')||'</a>'";
		$sql = "SELECT distinct
					CASE
						WHEN ptaf.pnbid is null THEN '<center> $btnAnexar $btnConsulta </center>'
						ELSE '<center> $btnConsulta </center>'
					END as acao,
					unv.unvsigla,
					'-' AS comdsc,
					(SELECT $lnkAno FROM livro.ptapnbe pta WHERE pta.unvid = unv.unvid AND pta.pnbstatusfinal != 't' AND pta.pnbindicadormecuniv = 'u' ORDER BY pta.pnbid DESC LIMIT 1) as verunv,
					(SELECT $lnkAno FROM livro.ptapnbe pta WHERE pta.unvid = unv.unvid AND pta.pnbstatusfinal != 't' AND pta.pnbindicadormecuniv = 'm' ORDER BY pta.pnbid DESC LIMIT 1) as vermec,
					(SELECT $lnkAno FROM livro.ptapnbe pta WHERE pta.unvid = unv.unvid AND pta.pnbstatusfinal = 't' ORDER BY pta.pnbid DESC LIMIT 1) as verfinal
				FROM livro.ptauniversidade unv
				LEFT JOIN livro.ptapnbe ptaf ON unv.unvid = ptaf.unvid AND ptaf.pnbstatusfinal = 't'
				LEFT JOIN livro.usuarioresponsabilidade ur ON unv.unvid = ur.unvid
				WHERE 1=1 AND unv.comid IS NULL
				" . implode(" ", $where) .
				" ORDER BY unv.unvsigla ";
	}
	
	
	$cabecalho = Array('Ação', 'Universidade', 'Disciplina', array('label' => 'Ultima Versão PTA', 'colunas' => array('Universidade', 'MEC', 'Final')));
	$alinhamento = Array('center', 'left', 'left', 'center', 'center', 'center' );
	$tamanho = Array('5%', '5%', '75%', '5%', '5%', '5%' );
	$db->monta_lista($sql, $cabecalho, 50, 20, 'N', 'left', 'N', '', $tamanho, $alinhamento, '');
?>
