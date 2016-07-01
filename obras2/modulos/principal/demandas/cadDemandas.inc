<?php

include_once(APPRAIZ . "obras2/modulos/principal/demandas/_funcoes_demandas.inc");

#Busca os perfis do usuario logado.
$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);

#Exibir Imagem
if ($_REQUEST['exibirfoto']) {
    echo '<img src="/slideshow/slideshow/verimagem.php?arqid=' . $_REQUEST['arqid'] . '"/>';
    die;
}

if ($_REQUEST['campoajax']) {

    $sql = "SELECT obrid as codigo , '(' || obrid || ') ' || obrnome as descricao FROM obras2.obras where obrid = " . $_REQUEST['campoajax'] . " and obrstatus = 'A' and obridpai IS NULL limit 1";
    // echo $sql;
    $dados = $db->pegaLinha($sql);
    $dados2 = json_encode($dados);
    echo $dados2;
    die;
}

#Salvar Demanda
function salvar()
{
    global $db;
    $_POST['dmdcpfsolicitante'] = $_POST['dmdsolicitante'];
    $_POST['dmdsolicitante'] = buscaDadosSolicitanteDemandas($_POST['dmdsolicitante'], 'usunome');
    $_POST['dmdurgente'] = $_POST['dmdurgente'] ? $_POST['dmdurgente'] : 'FALSE';
    $_POST['dmdcontrole'] = $_POST['dmdcontrole'] ? $_POST['dmdcontrole'] : 'FALSE';

    if ($_POST['dmdid'] != '') {
        $sql = "
                SELECT
                    dmdprioridade,
                    dmtid,
                    dmdurgente,
                    CASE 
                        WHEN dmdqtdpriorizado IS NULL 
                        THEN 1 
                    ELSE dmdqtdpriorizado 
                    END AS dmdqtdpriorizado
                FROM
                    obras2.demandas
                WHERE
                    dmdid = {$_POST['dmdid']};";

        $prioridadeAtual = $db->pegaLinha($sql);
        tiraFuroOrdem($prioridadeAtual['dmtid']);

        $mudouPrioridade = '';

        if ($prioridadeAtual['dmdprioridade'] != $_POST['dmdprioridade'] || $prioridadeAtual['dmdurgente'] != $_POST['dmdurgente']) {

            $prioridadeAtualizada = $prioridadeAtual['dmdprioridade'] < $_POST['dmdprioridade'] ? $prioridadeAtual['dmdprioridade'] : $_POST['dmdprioridade'];

            $where = Array();
            if ($prioridadeAtual['dmdprioridade'] != $_POST['dmdprioridade']) {
                $where[] = "dmdprioridade >= $prioridadeAtualizada";
            }

            if ($prioridadeAtual['dmdurgente'] != $_POST['dmdurgente']) {
                $where[] = "dmdid = {$_POST['dmdid']}";
            }

            $sqlHpd = "
                        INSERT INTO 
                            obras2.historico_priorizacao_demandas(
                        dmdid, 
                        dmdprioridade_atual, 
                        dmdprioridade_nova, 
                        dmdurgente_atual, 
                        dmdurgente_nova, 
                        dmdqtdpriorizado_atual, 
                        usucpf
                    )
                VALUES (	
                    {$_POST['dmdid']}, 
                    {$prioridadeAtual['dmdprioridade']}, 
                    {$_POST['dmdprioridade']}, 
                    " . ($prioridadeAtual['dmdurgente'] == 't' ? 'TRUE' : 'FALSE') . ", 
                        {$_POST['dmdurgente']}, 
                        {$prioridadeAtual['dmdqtdpriorizado']}, 
                        '{$_SESSION['usucpf']}'
                )
				RETURNING 
                    hpdid;
            ";

            $hpdid = $db->pegaUm($sqlHpd);

            $sqlHpd = "	
			    UPDATE 
                    obras2.demandas
                SET 
                    dmdqtdpriorizado = dmdqtdpriorizado +1 
			    WHERE 
                    " . implode(" OR ", $where);

            $db->executar($sqlHpd);
            $db->commit();

            atualizaEfeitoPriorizacao($prioridadeAtualizada, $hpdid, $_POST['dmdid']);
        }

        if ($prioridadeAtual['dmdprioridade'] != $_POST['dmdprioridade']) {
// 			$_POST['dmdprioridade'] = $prioridadeAtual['dmdprioridade'] < $_POST['dmdprioridade'] ? ( $prioridadeAtual['dmdprioridade'] == 1 ? $_POST['dmdprioridade']+1 : $_POST['dmdprioridade'] ) : $_POST['dmdprioridade']-1;
            $_POST['dmdprioridade'] = $prioridadeAtual['dmdprioridade'] < $_POST['dmdprioridade'] ? $_POST['dmdprioridade'] : $_POST['dmdprioridade'] - 1;
        }

        $_POST['obrid'] = (empty($_POST['obrid'])) ? 'null' : $_POST['obrid'];

        $sql = "
		    UPDATE 
                obras2.demandas
		    SET
                dmdnome = '" . str_replace("'", "", $_POST['dmdnome']) . "',
                dmoid = {$_POST['dmoid']},
                dmtid = {$_POST['dmtid']},
                dmdprioridade = {$_POST['dmdprioridade']},
                dmdsolicitante = '{$_POST['dmdsolicitante']}',
                dmdcpfresponsavel = '{$_POST['dmdcpfresponsavel']}',
                dmddescricao = '{$_POST['dmddescricao']}',
				dmdurgente = '{$_POST['dmdurgente']}',
				obrid = {$_POST['obrid']},
				dmdcontrole = '{$_POST['dmdcontrole']}',
				dmdultimaalteracao = now(),
				dmdcpfsolicitante = '{$_POST['dmdcpfsolicitante']}'
            WHERE
                dmdid = {$_POST['dmdid']}";

        $db->executar($sql);
        $db->commit();

        $sql = "SELECT dmdid FROM obras2.demandas ORDER BY dmdprioridade;";
        $dmdids = $db->carregarColuna($sql);

        if (is_array($dmdids)) {
            $sql = "";
            foreach ($dmdids as $k => $dmdid) {
                $sql .= "
                        UPDATE 
                            obras2.demandas
                        SET 
                            dmdprioridade = " . ($k + 1) . " 
                        WHERE 
                            dmdid = $dmdid;";
            }

            $db->executar($sql);
            $db->commit();
        }

        $dmdid = $_POST['dmdid'];

        tiraFuroOrdem($_POST['dmtid']);
    } else {
        require_once APPRAIZ . 'includes/classes/modelo/obras2/Email.class.inc';
        atualizaPrioridadeDemanda($_POST['dmdprioridade'], $_POST['dmtid']);

        $_POST['obrid'] = (empty($_POST['obrid'])) ? 'null' : $_POST['obrid'];

        $sql = "INSERT 
                INTO obras2.demandas
                    (
                        dmdnome, 
                        dmoid, 
                        dmtid, 
                        dmdprioridade, 
                        dmdsolicitante, 
                        dmdcpfresponsavel,
                        dmddescricao, 
                        dmdcpfcadastro, 
                        dmdultimaalteracao, 
                        dmdurgente,
                        dmdcontrole, 
                        dmdqtdpriorizado,
                        dmdcpfsolicitante,
                        obrid
                    )
                VALUES 
                    (
                        '{$_POST['dmdnome']}', 
                        {$_POST['dmoid']}, 
                        {$_POST['dmtid']}, 
                        {$_POST['dmdprioridade']}, 
                        '{$_POST['dmdsolicitante']}', 
                        '{$_POST['dmdcpfresponsavel']}',
                        '{$_POST['dmddescricao']}', 
                        '{$_SESSION['usucpf']}',
                        now(), 
                        {$_POST['dmdurgente']}, 
                        {$_POST['dmdcontrole']}, 
                        1,
                        '{$_POST['dmdcpfsolicitante']}',
                        {$_POST['obrid']}
                    )
            RETURNING dmdid;";

        $dmdid = $db->pegaUm($sql);
        $db->commit();
        $email = new Email();
        $email->enviaEmailDemanda(null, $dmdid);

        $docid = pegaDocidDemanda($dmdid);
    }


    # Busca estado da demanda no workflow
    $sqlBuscaEstadoDocumento = "
        SELECT 
	       esdid
	    FROM 
	       workflow.documento
	    WHERE
	       docid = (
	           SELECT 
	               docid 
                FROM 
	               obras2.demandas
               WHERE 
	               dmdid = {$dmdid}
		   )
    ";

    # Verifica se já foi enviado um e-mail sobre aquela demanda para o solicitante
    $sqlBuscaSituacaoEnvioEmail = "
        SELECT
	       dmdenvioemailsolicitante
	    FROM 
            obras2.demandas
        WHERE 
            dmdid = {$dmdid}
    ";

    $dmdenvioemailsolicitante = $db->pegaUm($sqlBuscaSituacaoEnvioEmail);
    $esdid = $db->pegaUm($sqlBuscaEstadoDocumento);

    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $campos = array("dmdid" => $dmdid);
    $arquivo = '';

    $file = new FilesSimec("demandasanexo", $campos, 'obras2');

    if ($_FILES["arquivo"]['name'] != '') {
        $arquivoSalvo = $file->setUpload($_FILES["arquivo"]['name']);
        $arquivo = $_FILES["arquivo"]['name'];
    }

    if ($dmdenvioemailsolicitante != 't' && $esdid == 829) {
        enviarEmailNotificacao($_POST, buscaDadosSolicitanteDemandas($_POST['dmdcpfsolicitante']), $dmdid, $arquivo);

        // Atualiza a flag informando o envio de e-mail
        $sql = "
            UPDATE 
    	       obras2.demandas
            SET
    	       dmdenvioemailsolicitante = 't'
    	    WHERE
    	       dmdid = {$dmdid};
	    ";

        $db->executar($sql);
        $db->commit();
    }

    $db->sucesso('principal/demandas/cadDemandas', '&dmdid=' . $dmdid);
    exit();
}

function anexarArquivo()
{

    global $db;

    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $campos = array("dmdid" => $_REQUEST['dmdid']);

    $file = new FilesSimec("demandasanexo", $campos, 'obras2');

    if ($_FILES["arquivo"]) {
        $arquivoSalvo = $file->setUpload($_FILES["arquivo"]['name']);
        if ($arquivoSalvo) {
            echo "
				<script>
					alert('Arquivo Anexado.');
					window.location.href = window.location.href;
				</script>";
        }
    }
}

function excluirArquivo()
{

    global $db;

    $sql = "
        UPDATE obras2.demandasanexo SET
            aqsstatus = 'I'
        WHERE
            arqid = '{$_REQUEST['arqid']}'";

    $db->executar($sql);
    $db->commit();

    echo "
		<script type=\"text/javascript\">
			alert('Arquivo Excluído.');
			document.location.href = document.location.href;
		</script>";
}

function downloadArquivo()
{

    require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $file = new FilesSimec(null, null, 'obras2');
    return $file->getDownloadArquivo($_REQUEST['arqid']);

    echo '
		<script type="text/javascript">
			document.location.href = document.location.href;
		</script>';
}

if ($_REQUEST['req']) {
    $_REQUEST['req']();
    die();
}

/**
 * @package name
 * @subpackage name
 *
 */
//include_once APPRAIZ . "includes/workflow.php";
include APPRAIZ . "includes/cabecalho.inc";
echo '<br>';

$db->cria_aba($abacod_tela, $url, $parametros);

monta_titulo('Demanda', '');

global $db;

$habilitado = 'N';

if ($_REQUEST['dmdid'] != '') {

    $sql = "SELECT
				*
			FROM
				obras2.demandas
			WHERE
				dmdid = {$_REQUEST['dmdid']}";

    $demanda = $db->pegaLinha($sql);
    $esdid = pegaEstadoAtualDocumento($demanda['docid']);
}

$arrEstado = Array(PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO, PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA);

if (((in_array($esdid, $arrEstado) || $esdid == '') && possui_perfil(PAR_PERFIL_DEMANDANTE_PAR)) || $db->testa_superuser()) {
    $habilitado = 'S';
}
?>

<link href="../includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>

<!-- Add mousewheel plugin -->
<script type="text/javascript" src="/library/fancybox/helpers/jquery.mousewheel-3.0.6.pack.js"></script>

<!-- Add fancyBox main JS and CSS files -->
<script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>
<link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen"/>

<!-- Add Button helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-buttons.css"/>
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-buttons.js"></script>

<!-- Add Thumbnail helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-thumbs.css"/>
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-thumbs.js"></script>

<!-- Add Media helper (this is optional) -->
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-media.js"></script>

<style type="text/css">
    .fancybox img {
        box-shadow: 1px 1px 4px 1px #000;
        width: 10px !important;
        height: 10px !important;
        margin-right: 10px;
    }
</style>

<form method="post" name="form_save" id="form_save" action="" class="formulario" enctype="multipart/form-data">
<input type="hidden" name="dmdid" id="dmdid" value="<?= $demanda['dmdid'] ?>"/>
<input type="hidden" name="arqid" id="arqid" value=""/>
<input type="hidden" name="req" id="req" value=""/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
<td>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<?php if ($_REQUEST['dmdid']) : ?>
    <tr>
        <td class="SubTituloDireita" width="20%"><label for="dmdnome">Código:</label></td>
        <td width="80%"><?php echo $_REQUEST['dmdid'] ?></td>
    </tr>
<?php endif; ?>
<tr>
    <td class="SubTituloDireita" width="20%"><label for="dmdnome">Assunto:</label></td>
    <td width="80%"><?php echo campo_texto('dmdnome', 'S', $habilitado, 'Descrição Ação', 80, 50, '', '', '', '', '', 'id="dmdnome" required', '', $demanda['dmdnome']); ?></td>
</tr>
<tr>
    <td class="SubTituloDireita" valign="top"><label for="descricao_detalhada">Origem: </label></td>
    <td>
        <?php $sql = "SELECT dmoid as codigo , dmonome as descricao FROM obras2.demandas_origem"; ?>
        <?php echo $db->monta_combo("dmoid", $sql, $habilitado, "Selecione", '', '', '', '520', 'S', 'dmoid', '', $demanda['dmoid'], '', 'required'); ?>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita" valign="top"><label for="descricao_detalhada">Tipo: </label></td>
    <td>
        <?php $sql = "SELECT dmtid as codigo , dmtnome as descricao FROM obras2.demandastipo"; ?>
        <?php echo $db->monta_combo("dmtid", $sql, $habilitado, "Selecione", '', '', '', '520', 'S', 'dmtid', '', $demanda['dmtid'], '', 'required'); ?>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita"><label for="assunto">Prioridade:</label></td>
    <td>
        <input type="hidden" id="prioridadeAntiga" name="prioridadeAntiga" value="<?= $demanda['dmdprioridade'] ?>"/>
        <?php echo campo_texto('dmdprioridade', 'S', $habilitado, 'Prioridade', 3, 3, '###', '', '', '', '', 'id="dmdprioridade" required', '', $demanda['dmdprioridade']); ?>
    </td>
</tr>
<tr>
    <td class="SubTituloDireita"><label for="assunto">ID da Obra:</label></td>
    <td>
        <?php echo campo_texto('obrid', 'N', $habilitado, 'ID da obra', 15, 15, '###############', '', '', '', '', 'id="campoajax"', '', $demanda['obrid']); ?>

        <span id="resultado" style="display: none;padding: 15px 0;color: #0043C1;margin-left: 20px;font-size: 14px;"></span>
        <span id="resultado_negativo" style="display: none;padding: 15px 0;color: #ce151c;margin-left: 20px;font-size: 14px;">Não foi encontrada obra com esse ID</span>

    </td>
</tr>
<tr>
    <td class="SubTituloDireita"><label for="assunto">Urgente?:</label></td>
    <td>
        <input type="radio" name="dmdurgente" id="dmdurgentesim"
               value="TRUE"   <?= ($demanda['dmdurgente'] == 't' ? 'checked' : '') ?> /> SIM&nbsp;
        <input type="radio" name="dmdurgente" id="dmdurgentenao"
               value="FALSE"  <?= ($demanda['dmdurgente'] != 't' ? 'checked' : '') ?>/> NÃO
    </td>
</tr>
<?php
if (in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil)) {
    ?>
    <tr>
        <td class="SubTituloDireita"><label for="assunto">Controle?:</label></td>
        <td>
            <input type="radio" name="dmdcontrole" id="dmdcontrolesim"
                   value="TRUE"   <?= ($demanda['dmdcontrole'] == 't' ? 'checked' : '') ?> /> SIM&nbsp;
            <input type="radio" name="dmdcontrole" id="dmdcontrolenao"
                   value="FALSE"  <?= ($demanda['dmdcontrole'] != 't' ? 'checked' : '') ?>/> NÃO
        </td>
    </tr>
<?php
}
?>
<tr>
    <td class="SubTituloDireita"><label for="assunto">Solicitante:</label></td>
    <td>
        <?php
        $sql = "
                            SELECT DISTINCT
                            	usu.usucpf as codigo,
                                usu.usunome as descricao
                            FROM 
                            	seguranca.perfilusuario pfl
                            	INNER JOIN seguranca.usuario usu ON usu.usucpf = pfl.usucpf
                            	INNER JOIN seguranca.usuario_sistema uss ON uss.usucpf = usu.usucpf
                            WHERE 
                            	pfl.pflcod IN (1276, 944, 932, 1341, 1342, 1278)
                            	AND uss.sisid = 147
                            	AND susstatus = 'A'
                            ORDER BY
                                usu.usunome;";

        $db->monta_combo("dmdsolicitante", $sql, $habilitado, "Selecione", '', '', '', '520', 'S', 'dmdsolicitante', '', $demanda['dmdcpfsolicitante'], '', 'required class="sel_chosen"')
        ?>
    </td>
</tr>
<?php if ($db->testa_superuser()) { ?>
    <tr>
        <td class="SubTituloDireita"><label for="assunto">Responsável pela demanda:</label></td>
        <td>
            <?php # echo campo_texto('dmdcpfresponsavel', 'N', $habilitado, 'Solicitante', 80, 255, '', '', '', '', '', 'id="dmdcpfresponsavel" required', '', $demanda['dmdcpfresponsavel']); ?>
            <?php
            $sql = "
                SELECT DISTINCT
                    usu.usucpf as codigo,
                    usu.usunome as descricao
                FROM  seguranca.perfilusuario pfl
                INNER JOIN seguranca.usuario usu ON usu.usucpf = pfl.usucpf
                INNER JOIN seguranca.usuario_sistema uss ON uss.usucpf = usu.usucpf
                WHERE  pfl.pflcod IN (1276, 944, 932, 1341, 1342, 1278)
                    AND uss.sisid = 147
                    AND susstatus = 'A'
                ORDER BY usu.usunome;";

            $db->monta_combo("dmdcpfresponsavel", $sql, $habilitado, "Selecione", '', '', '', '520', 'S', 'dmdcpfresponsavel', '', $demanda['dmdcpfresponsavel'], '', 'required class="sel_chosen"')
            ?>
        </td>
    </tr>
<?php } ?>
<tr>
    <td class="SubTituloDireita" valign="top"><label for="dmddescricao">Descrição detalhada: </label></td>
    <td>
        <?php echo campo_textarea('dmddescricao', 'S', $habilitado, 'Descrição Detalhada', 130, 10, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição detalhada', $demanda['dmddescricao'], array('id' => 'dmddescricao')); ?>
    </td>
</tr>
<?php if ($esdid == PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA) { ?>
    <?php
    $sql = "SELECT
                                    cmddsc
                                FROM
                                    workflow.comentariodocumento
                                WHERE
                                    hstid = (
                                        SELECT
                                            max(hstid)
                                        FROM
                                            workflow.historicodocumento
                                        WHERE
                                            docid = {$demanda['docid']}
                                    );
				";
    $cmddsc = $db->pegaUm($sql);

    if ($cmddsc == '') {
        $cmddsc = 'Não informado';
    }
    ?>
    <tr>
        <td class="SubTituloDireita" valign="top"><label for="dmddescricao">Motivo da Rejeição: </label></td>
        <td>
            <?php echo campo_textarea('cmddsc', 'N', 'N', 'Descrição Detalhada', 150, 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição detalhada', $cmddsc, array('id' => 'dmddescricao')); ?>
        </td>
    </tr>
<?php } ?>
<tr>
    <td class="SubTituloDireita" valign="top"><label for="dmddescricao">Arquivos: </label></td>
    <td>
        <input type="file" name="arquivo" id="arquivo" class=""/>
        <?php if ($demanda['dmdid'] != '') { ?>
            <input id="button_anexa" type="button" value="Anexar"/>
        <?php } ?>
        <br><br>
        <?php
        if ($db->testa_superuser()) {
            $acoes = '\'
									<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="excluirArquivo">
										<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir">
									</a>&nbsp;
									<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="downloadArquivo">
										<img src=\"/imagens/anexo.gif\" border=0 title=\"Download">
									</a>&nbsp;
									\'||
									case when substring(arqtipo, 1,5) = \'image\' then
										\'<a class="fancybox fancybox.ajax" href="obras2.php?modulo=principal/demandas/cadDemandas&acao=A&dmdid=' . $demanda['dmdid'] . '&exibirfoto=1&arqid=\'|| arq.arqid ||\'" data-fancybox-group="gallery" title="\' || arq.arqdescricao || \'">
														\' || arq.arqdescricao || \'</a>\'
									else
										\'<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="downloadArquivo">
											\' || arq.arqdescricao || \'
											</a>\' end
									';
        } elseif ($habilitado == 'S') {
            $acoes = '
							(
							CASE
							WHEN doc.esdid = ' . PAR_WF_DEMANDA_ESTADO_EM_CADASTRAMENTO . ' OR doc.esdid = ' . PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA . '
							THEN
								\'<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="excluirArquivo">
									<img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir">
									</a>&nbsp;
									\'||
									case when substring(arqtipo, 1,5) = \'image\' then
										\'<a class="fancybox fancybox.ajax" href="obras2.php?modulo=principal/demandas/cadDemandas&acao=A&dmdid=' . $demanda['dmdid'] . '&exibirfoto=1&arqid=\'|| arq.arqid ||\'" data-fancybox-group="gallery" title="\' || arq.arqdescricao || \'">
														\' || arq.arqdescricao || \'</a>\'
									else
										\'<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="downloadArquivo">
											\' || arq.arqdescricao || \'
											</a>\' end
							ELSE
	
									case when substring(arqtipo, 1,5) = \'image\' then
										\'<a class="fancybox fancybox.ajax" href="obras2.php?modulo=principal/demandas/cadDemandas&acao=A&dmdid=' . $demanda['dmdid'] . '&exibirfoto=1&arqid=\'|| arq.arqid ||\'" data-fancybox-group="gallery" title="\' || arq.arqdescricao || \'">
														\' || arq.arqdescricao || \'</a>\'
									else
										\'<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="downloadArquivo">
											\' || arq.arqdescricao || \'
											</a>\' end
							END
							)';
        } else {
            $acoes = 'case when substring(arqtipo, 1,5) = \'image\' then
										\'<a class="fancybox fancybox.ajax" href="obras2.php?modulo=principal/demandas/cadDemandas&acao=A&dmdid=' . $demanda['dmdid'] . '&exibirfoto=1&arqid=\'|| arq.arqid ||\'" data-fancybox-group="gallery" title="\' || arq.arqdescricao || \'">
														\' || arq.arqdescricao || \'</a>\'
									else
										\'<a style="margin: 0 -5px 0 5px;" href="#" id="\' || arq.arqid || \'" class="downloadArquivo">
											\' || arq.arqdescricao || \'
											</a>\' end';
        }

        if ($demanda['dmdid'] != '') {
            $cabecalho = array('Arquivo');

            $sql = "SELECT
									$acoes as acoes
								FROM
									obras2.demandasanexo aqs
								INNER JOIN public.arquivo arq ON arq.arqid = aqs.arqid
								INNER JOIN obras2.demandas dmd ON dmd.dmdid = aqs.dmdid
								INNER JOIN workflow.documento doc ON doc.docid = dmd.docid
								WHERE
									aqsstatus = 'A'
									AND dmd.dmdid = {$demanda['dmdid']}";

            $arrArquivos = $db->carregar($sql);
            $arrArquivos = $arrArquivos ? $arrArquivos : array();

            if (is_array($arrArquivos)) {
                $db->monta_lista($arrArquivos, $cabecalho, 50, 10, '', 'center', 'N', '', '', '', '', '');
            }
        }
        ?>

    </td>
</tr>
</table>
</td>
<td valign="top">
    <?php
    if ($demanda['docid'] && (in_array(PAR_PERFIL_SUPER_USUARIO, $arrayPerfil) || in_array(PAR_PERFIL_DEMANDANTE_PAR, $arrayPerfil) || in_array(PAR_PERFIL_GESTOR_MEC, $arrayPerfil))) {
        require_once APPRAIZ . 'includes/workflow.php';
        // Verifica qual estado do documento.. se for em diligência mostra o box personalizado

        if (in_array($esdid, array(PAR_WF_DEMANDA_ESTADO_EM_DILIGENCIA, PAR_WF_DEMANDA_ESTADO_AGUARDANDO_VALIDACAO))) {
            $modificacao = wf_pegarUltimaDataModificacao($demanda['docid']);
            $usuario = wf_pegarUltimoUsuarioModificacao($demanda['docid']);
            $comentario = trim(substr(wf_pegarComentarioEstadoAtual($demanda['docid']), 0, 50)) . "...";
            $estadoAtual = wf_pegarEstadoAtual($demanda['docid']);
            $estados = wf_pegarProximosEstados($demanda['docid'], array());
            ?>

            <table border="0" cellpadding="3" cellspacing="0"
                   style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">

                <tr style="background-color: #c9c9c9; text-align:center;">
                    <td style="font-size:7pt; text-align:center;">
                                    <span title="estado atual">
                                        <b>Estado atual</b>
                                    </span>
                    </td>
                </tr>
                <tr style="text-align:center;">
                    <td style="font-size:7pt; text-align:center;">
                                    <span title="estado atual">
                                        <?php echo $estadoAtual['esddsc'] ?>
                                    </span>
                    </td>
                </tr>

                <tr style="background-color: #c9c9c9; text-align: center;">
                    <td style="font-size: 7pt; text-align: center;">
                        <span title="Análise de Turmas"><strong>Ação</strong></span>
                    </td>
                </tr>

                <?php
                foreach ($estados as $estado) {
                    // Verifica se o estado do documento de destino é igual a 1215 'Devolver Demanda' ou
                    if ($estado['esdid'] == 1215 || $estado['esdid'] == 1208) {
                        $onClick = 'onclick="alteraEstadoWorkflowDiligencia(\'' . $demanda['dmdid'] . '\', \'' . $demanda['docid'] . '\');"';
                    } else {
                        $onClick = 'onclick="wf_alterarEstado(\'' . $estado['aedid'] . '\', \'' . $demanda['docid'] . '\', \'' . $estado['esdid'] . '\', \'' . $estado['aeddscrealizar'] . '\' ); window.location.href = window.location;"';
                    }

                    $action = wf_acaoPossivel2($demanda['docid'], $estado['aedid'], array());

                    if ($action === true) {
                        ?>
                        <tr>
                            <td id="td_acao_<?= $estado['aedid'] ?>"
                                style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;"
                                onmouseover="this.style.backgroundColor = '#ffffdd';"
                                onmouseout="this.style.backgroundColor = '';">
                                <a href="#" alt="<?php echo $estado['aeddscrealizar'] ?>"
                                   title="<?php echo $estado['aeddscrealizar'] ?>" <?php echo $onClick ?>><?php echo $estado['aeddscrealizar'] ?></a>
                            </td>
                        </tr>
                    <?php
                    }
                }
                ?>

                <tr style="background-color: #c9c9c9; text-align:center;">
                    <td style="font-size:7pt; text-align:center;">
                                    <span title="estado atual">
                                        <b>Histórico</b>
                                    </span>
                    </td>
                </tr>
                <tr style="text-align:center;">
                    <td style="font-size:7pt; border-top: 2px solid #d0d0d0;">
                        <img
                            style="cursor: pointer;"
                            src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/fluxodoc.gif"
                            title="<?php echo $usuario['usunome'] . " - " . $modificacao . " - " . simec_htmlentities($comentario); ?>"
                            onclick="wf_exibirHistorico('<?php echo $demanda['docid'] ?>');"
                            />
                    </td>
                </tr>

            </table>
        <?php
        } else {
            wf_desenhaBarraNavegacao($demanda['docid'], array('docid' => $demanda['docid']));
        }
    }
    ?>
</td>
</tr>
<tr bgcolor="#dcdcdc">
    <td style="text-align:center" colspan="2">
        <?php if ($habilitado == 'S') { ?>
            <input id="button_save" type="button" value="Salvar"/>
        <?php } ?>
    </td>
</tr>
</table>
</form>
<script>
    $1_11(document).ready(function () {
        $1_11('select[name="obrid"]').chosen();
    });

</script>
<script>
jQuery(document).ready(function () {
    jQuery('.fancybox').fancybox();

    function pegaPrioridadeMaxima() {
        var dmtid = jQuery('#dmtid').val();
        var retorno = '0';
        if (dmtid !== '') {
            jQuery.ajax({
                type: "POST",
                data: {req: 'pegaPrioridadeMaxima'},
                url: 'obras2.php?modulo=principal/demandas/cadDemandas&acao=A&dmtid=' + dmtid,
                async: false,
                success: function (retornoajax) {
                    retorno = retornoajax;
                }
            });
        }
        return retorno;
    }

    function pegaPrioridadeMinima() {
        var dmtid = jQuery('#dmtid').val();
        var retorno = '0';
        if (dmtid != '') {
            jQuery.ajax({
                type: "POST",
                data: {req: 'pegaPrioridadeMinima'},
                url: 'obras2.php?modulo=principal/demandas/cadDemandas&acao=A&dmtid=' + dmtid,
                async: false,
                success: function (retornoajax) {
                    retorno = retornoajax;
                }
            });
        }

        return retorno;
    }

    function testaAtendimentoDemandaPorPrioridade() {
        var dmtid = jQuery('#dmtid').val();
        var dmdprioridade = jQuery('#dmdprioridade').val();
        var retorno;
        jQuery.ajax({
            type: "POST",
            data: {req: 'testaAtendimentoDemandaPorPrioridade'},
            url: 'obras2.php?modulo=principal/demandas/demandas&acao=A&dmtid=' + dmtid + '&dmdprioridade=' + dmdprioridade,
            async: false,
            success: function (retornoajax) {
                retorno = trim(retornoajax);
            }
        });

        if (retorno) {
            return true;
        } else {
            return false;
        }
    }

    function pegaPrioridadeMinimaSemAtendimento(prioridadeInvalida) {
        var dmtid = jQuery('#dmtid').val();
        var retorno;
        jQuery.ajax({
            type: "POST",
            data: {req: 'pegaPrioridadeMinimaSemAtendimento'},
            url: 'obras2.php?modulo=principal/demandas/demandas&acao=A&dmtid=' + dmtid + '&prioridadeInvalida=' + prioridadeInvalida,
            async: false,
            success: function (retornoajax) {
                retorno = trim(retornoajax);
            }
        });

        return retorno;
    }

    jQuery('#dmtid').change(function () {
        var dmdid = jQuery('#dmdid').val();
        console.log(dmdid);
        if (dmdid === '') {
            var prioridadeMax = parseInt(pegaPrioridadeMaxima());
            prioridadeMax = prioridadeMax + 1;
            jQuery('#dmdprioridade').val(prioridadeMax);
        }
    });

    jQuery('#button_save').click(function () {
        var dmdprioridade = parseInt(jQuery('#dmdprioridade').val());
        var prioridadeMax = parseInt(pegaPrioridadeMaxima());
        var dmdid = jQuery('#dmdid').val();
        if (dmdid === '') {
            prioridadeMax = prioridadeMax + 1;
        }
        if (dmdprioridade != prioridadeMax && dmdprioridade != jQuery('#prioridadeAntiga').val()) {
            if (!confirm('Essa prioridade já existe para esse tipo de demanda.\n' +
                'Neste caso, todas demandas que tem prioridade menor ou igual a essa serão atualizadas para ficar abaixo dessa demanda.' +
                '\nDeseja salvar assim mesmo?')) {
                return false;
            }
        }
        var erro = false;
        jQuery('.obrigatorio').each(function () {
            if (jQuery(this).val() == '') {
                alert('Campo Obrigatório.');
                jQuery(this).focus();
                erro = true;
                return false;
            }
        });
        if (erro) {
            return false;
        }
        jQuery('#req').val('salvar');
        jQuery('#form_save').submit();
    });

    jQuery('#button_anexa').click(function () {
        var dmdid = jQuery('#dmdid').val();
        if (dmdid != '') {
            jQuery('#req').val('anexarArquivo');
            jQuery('#form_save').submit();
        }
    });

    jQuery('.downloadArquivo').click(function () {
        window.open('obras2.php?modulo=principal/demandas/cadDemandas&acao=A&req=downloadArquivo&arqid=' + jQuery(this).attr('id'), 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    });

    jQuery('.excluirArquivo').click(function () {
        if (confirm('Deseja excluir este arquivo?')) {
            jQuery('#req').val('excluirArquivo');
            jQuery('#arqid').val(jQuery(this).attr('id'));
            jQuery('#form_save').submit();
        }
    });

    jq('.sel_chosen').chosen({allow_single_deselect: true});

    alteraEstadoWorkflowDiligencia = function (dmdid, docid) {
        window.open('obras2.php?modulo=principal/demandas/popUpAlterarEstadoWorkflow&acao=A&dmdid=' + dmdid + '&docid=' + docid, 'page', "height=650,width=900,scrollbars=yes,top=50,left=200");
    }

    wf_exibirHistorico = function (docid) {
        var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/historico.php' +
            '?modulo=principal/tramitacao' +
            '&acao=C' +
            '&docid=' + docid;
        window.open(
            url,
            'alterarEstado',
            'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
        );
    }
    wf_alterarEstado = function (aedid, docid, esdid, acao, recarregar) {
        if (acao)
            acao = acao.toLowerCase();
        if (!confirm('Deseja realmente ' + acao + ' ?')) {
            return;
        }
        var url = 'http://<?php echo $_SERVER['HTTP_HOST'] ?>/geral/workflow/alterar_estado.php' +
            '?aedid=' + aedid +
            '&docid=' + docid +
            '&esdid=' + esdid;
        var janela = window.open(
            url,
            'alterarEstado',
            'width=550,height=520,scrollbars=no,scrolling=no,resizebled=no'
        );

        if (!recarregar) {
            janela.focus();
        } else {
            janela.opener.location.reload();
            janela.close();
        }
    }
});


$(document).ready(function () {

    $("#campoajax").keyup(function () {

        if ($("#campoajax").val().length > 3) {
            $.getJSON(window.location, {campoajax: $("#campoajax").val()})
                .done(function (json) {
                    $("#resultado").html(json.descricao);
                    $("#resultado").show();
                    $("#obrid_hidden").val($("#campoajax").val());
                    $("#resultado_negativo").hide();
                    $("#button_save").show();
                    if(json == false){
                        $("#resultado_negativo").show();
                        $("#resultado").hide();
                        $("#obrid_hidden").val("0");
                        $("#button_save").hide();


                    };                })

        }


        if($("#campoajax").val().length == 0){
            $("#resultado_negativo").hide();
            $("#resultado").hide();
            $("#button_save").show();
        }
        else{
            $("#resultado_negativo").show();
            $("#resultado").hide();
            $("#button_save").hide();
        }
        ;
    }).keyup();;

});

</script>