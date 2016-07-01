<?PHP

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
}


include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Mais Médicos - MEC', 'Cadastro da Mantenedora');

$abacod_tela = '';
$url = '';
$parametros = '';

//$db->cria_aba($abacod_tela, $url, $parametros);

unset($_SESSION['maismedicomec']['mntid']);

$perfil = pegaPerfilGeral();
?>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

    function atualizaComboMunicipioMantenedora(estuf) {
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicao=atualizaComboMunicipioMantenedora&estuf=" + estuf,
            async: false,
            success: function(resp) {
                $('#td_combo_municipio').html(resp);
            }
        });
    }

    function cadastraMantenedora() {
        window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/cad_mantenedora&acao=A';
    }

    function editarMantenedora(mntid) {
        //window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/cad_mantenedora&acao=A&mntid=' + mntid;
        window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/condicoes_participacao&acao=A&mntid=' + mntid;
    }

    function pesquisarServidor(param) {
        if (trim(param) == 'fil') {
        	$('#submite').val('OK');
        	$('#tipoxls').val('');
            $('#formulario').submit();
        } else {
           location.href = 'maismedicomec.php?modulo=principal/mantenedora/lista_grid_mantenedora&acao=A';
        }
    }

    function exportarXls() {
        $('#submite').val('OK');
        $('#tipoxls').val('OK');
        $('#formulario').submit();
    }

</script>


<form action="" method="POST" id="formulario" name="formulario">

    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="submite" name="submite" value=""/>
    <input type="hidden" id="tipoxls" name="tipoxls" value=""/>

	<?if(!in_array(PERFIL_IES, $perfil)){?>

	    <table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">

	        <tr>
	            <td class ="SubTituloDireita" width="35%">Mantenedora:</td>
	            <td>
	                <?= campo_texto('mntrazaosocial', 'N', 'S', '', 89, 100, '', '', '', '', 0, 'id="mntrazaosocial"', '', $_POST['mntrazaosocial'], null, '', null); ?>
	            </td>
	        </tr>
	        <tr>
	            <td class ="SubTituloDireita">Sigla:</td>
	            <td>
	                <?= campo_texto('mntsigla', 'N', 'S', '', 49, 20, '', '', '', '', 0, 'id="mntsigla"', '', $_POST['mntsigla'], null, '', null); ?>
	            </td>
	        </tr>
	        <tr>
	            <td class ="SubTituloDireita">CNPJ:</td>
	            <td>
	                <?= campo_texto('mntcnpj', 'N', 'S', '', 49, 14, '', '', '', '', 0, 'id="mntcnpj"', '', $_POST['mntcnpj'], null, '', null); ?>
	            </td>
	        </tr>
	        <tr>
	            <td class ="SubTituloDireita">Unidade Federativa (UF):</td>
	            <td>
	                <?PHP
	                $sql = "
	                        SELECT  estuf AS codigo,
	                                estuf ||' - '|| estdescricao AS descricao
	                        FROM territorios.estado
	                        ORDER BY estuf
	                    ";
	                $db->monta_combo("estuf", $sql, 'S', 'Selecione...', 'atualizaComboMunicipioMantenedora', '', '', 300, 'N', 'estuf', false, $_POST['estuf'], null);
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class ="SubTituloDireita">Município:</td>
	            <td id="td_combo_municipio">
	                <?PHP
	                $sql = "
	                        SELECT  m.muncod AS codigo,
	                                m.mundescricao AS descricao
	                        FROM territorios.municipio AS m
	                        LEFT JOIN territorios.estado AS u ON u.estuf = m.estuf
	                        ORDER BY u.estuf, m.mundescricao
	                    ";
	                $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', 300, 'N', 'muncod', false, $_POST['muncod'], null);
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class ="SubTituloDireita">Situação:</td>
	            <td id="td_combo_municipio">
	                <?PHP
	                $sql = "
	                        SELECT  1 AS codigo, 'Não inicado' AS descricao
	                        union
	                        SELECT  esdid AS codigo, esddsc AS descricao
	                        from workflow.estadodocumento
	                        where tpdid = ".WF_FLUXO_MANTENEDORA."
	                        ORDER BY 1
	                    ";
	                $db->monta_combo("situacao", $sql, 'S', 'Selecione...', '', '', '', 300, 'N', 'situacao', false, $_POST['situacao'], null);
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class ="SubTituloDireita">Mantidas com Recurso:</td>
	            <td id="td_matida_recurso">
                        <input type="radio" id="mantida_recurso_t" name="mantida_recurso" value="T" checked="checked"> TODOS
                        <input type="radio" id="mantida_recurso_s" name="mantida_recurso" value="S"> SIM
                        <input type="radio" id="mantida_recurso_n" name="mantida_recurso" value="N"> NÂO
	            </td>
	        </tr>
	        <tr>
	            <td class="SubTituloCentro" colspan="2">
	                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarServidor('fil');"/>
	                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarServidor('ver');"/>
	                <input type="button" name="expostarxls" value="Exportar XLS" onclick="exportarXls();" >
	            </td>
	        </tr>
	    </table>

	 <?}?>

</form>

<?//if(!in_array(PERFIL_IES, $perfil)){?>
	<!--
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	    <tr>
	        <td>
	            <span style="cursor: pointer" onclick="cadastraMantenedora();" title="Novo Professor" >
	                <img align="absmiddle" src="/imagens/gif_inclui.gif" width="11px"/> &nbsp; Cadastrar Nova Mantenedora
	            </span>
	        </td>
	    </tr>
	</table>
	 -->
<?//}?>

<br>

<?PHP
    if($_POST['submite']=='OK'){
	if( $_POST['mntrazaosocial'] ){
	    $where .= " AND public.removeacento(m.mntdsc) ilike public.removeacento( ('%" . trim($_POST['mntrazaosocial']) . "%') )";
	}

	if( $_POST['mntsigla'] ){
	    $where .= " AND public.removeacento(mm.mntsigla) ilike public.removeacento( ('%" . trim($_POST['mntsigla']) . "%') )";
	}

	if( $_POST['srpnumcpf'] ){
	    $where .= " AND m.mntcnpj = '" . str_replace('.', '', str_replace('/', '', str_replace('-', '', $_POST['mntcnpj']))) . "'";
	}

	if( $_POST['estuf'] ){
	    $where .= " AND mm.estuf = '" . $_POST['estuf'] . "'";
	}

	if( $_POST['muncod'] ){
	    $where .= " AND mm.muncod = '" . $_POST['muncod'] . "'";
	}

	if( $_POST['situacao'] ){
            if ($_POST['situacao'] == '1') {
            $where .= " AND doc.docid is null ";
            }else{
            $where .= " AND doc.esdid in (".$_POST['situacao'].") ";
            }
	}

        if( $_POST['mantida_recurso'] != 'T' ){
            if( $_POST['mantida_recurso'] == 'S' ){
                $where .= " AND (CASE WHEN (pimdscresposta is not null OR pimdscresposta <> '') THEN 'S' ELSE 'N' END) = 'S'";
            }else{
                $where .= " AND (CASE WHEN (pimdscresposta is not null OR pimdscresposta <> '') THEN 'S' ELSE 'N' END) = 'N'";
            }
        }
    }


if (in_array(PERFIL_IES, $perfil)) {
   $joinIES = " INNER JOIN maismedicomec.usuarioresponsabilidade ur on ur.mntid = m.mntid and ur.rpustatus='A' and ur.pflcod = ".PERFIL_IES." and ur.usucpf='{$_SESSION['usucpf']}' ";
   //$andPerfil = " and pu.pflcod = ".PERFIL_IES;
}

if (in_array(PERFIL_HOSPITAL, $perfil)) {
   $joinIES = " INNER JOIN maismedicomec.usuarioresponsabilidade ur on ur.mntid = m.mntid and ur.rpustatus='A' and ur.pflcod = ".PERFIL_HOSPITAL." and ur.usucpf='{$_SESSION['usucpf']}' ";
   //$andPerfil = " and pu.pflcod = ".PERFIL_IES;
}

if (in_array(PERFIL_MM_MEC_ADMINISTRADOR, $perfil) || in_array(PERFIL_MM_MEC_SUPER_USUARIO, $perfil)) {
   $joinIES = " INNER JOIN maismedicomec.usuarioresponsabilidade ur on ur.mntid = m.mntid and ur.rpustatus='A' ";
   //$andPerfil = " and pu.pflcod = ".PERFIL_IES;
}

if (in_array(PERFIL_MM_MEC_REVISOR, $perfil)) {
   $joinIES = " INNER JOIN maismedicomec.usuarioresponsabilidade ur on ur.mntid = m.mntid and ur.rpustatus='A' and ur.usucpf='{$_SESSION['usucpf']}'";
}


    $acao = "
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarMantenedora('||m.mntid||');\" title=\"Editar Professor\" >
    ";

    $sql = "
        SELECT distinct
                        '{$acao}' as acao,
                 case when mm.mntrazaosocial is not null then
                                mm.mntrazaosocial
                             else
                                m.mntdsc
                        end as razaosocial,
        mm.mntsigla,
        m.mntcnpj,
        mm.estuf,
        mu.mundescricao,
        mm.mnttelefone01,
        mm.mntemail, --,  pe.pfldsc
        CASE WHEN doc.docid is null THEN
                        'Não iniciado'
                 ELSE
                        (select esddsc from workflow.estadodocumento where esdid = doc.esdid)
        END as situacao
        FROM gestaodocumentos.mantenedoras AS m

        LEFT JOIN maismedicomec.mantenedora AS mm ON  m.mntid = mm.mntid
        LEFT JOIN workflow.documento AS doc ON doc.docid = mm.docid
        LEFT JOIN territorios.municipio AS mu ON  mu.muncod = mm.muncod

        LEFT JOIN (
            SELECT mt.mntid, pimdscresposta
            FROM maismedicomec.mantida AS m
            JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
            LEFT JOIN maismedicomec.parecerinscricaomantida p ON p.mnmid = mt.mnmid

            UNION

            SELECT mt.mntid, pimdscresposta
            FROM maismedicomec.mantidacandidata AS m
            JOIN maismedicomec.mantidamunicipiocandidata AS mt ON mt.mtdid = m.mtdid
            LEFT JOIN maismedicomec.parecerinscricaomantida p ON p.mnmid = mt.mnmid
        ) AS resp ON resp.mntid = mm.mntid

        $joinIES

        WHERE 1 = 1 {$where}
        ORDER BY 2
    ";
    $cabecalho = array("Ação", "Razão Social", "Sigla", "CNPJ", "UF", "Municípo", 'Telefone', 'E-mail', 'Situação');
    $alinhamento = Array('center', '', '', '', '', '', '', '', '');
    $tamanho = Array('2%', '', '', '', '', '', '', '');

    if($_POST['tipoxls']=='OK'){
        ob_clean();
        header("Content-type: application/vnd.ms-excel");
        header("Content-type: application/force-download");
        header("Content-Disposition: attachment; filename=simec_maismedicosmec_mantenedora.xls");
        header("Pragma: no-cache");
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
    }else{
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
    }
?>


<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script>
    var countModal = 1;

    function montaShowModal() {
        var campoTextArea = ''+
                '<p align=\'justify\'><CENTER><FONT COLOR=RED><B>ATENÇÃO</B></FONT></CENTER>'+
                '<br />Senhor(a) Representante da IES,'+
                '<br /><br />Conforme Edital nº 6/2014/SERES/MEC, de 23/12/2014, item 2, apartir de zero hora do dia 24/01/2015, o sistema será bloqueado para inclusão de novas informações, permitindo somente consulta.'+
                '<br /><br /><br /><br /></p>';
        var alertaDisplay = '<div class="titulo_box" >'+campoTextArea+'</div><div class="links_box" ><center><input type="button" onclick=\'closeMessage(); return false \' value="Fechar" /></center>';
        displayStaticMessage(alertaDisplay,false);
        return false;
    }

    function displayStaticMessage(messageContent,cssClass) {
        messageObj = new DHTML_modalMessage();	// We only create one object of this class
        messageObj.setShadowOffset(5);	// Large shadow
        messageObj.setHtmlContent(messageContent);
        messageObj.setSize(520,180);
        messageObj.setCssClassMessageBox(cssClass);
        messageObj.setSource(false);	// no html source since we want to use a static message here.
        messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes
        messageObj.display();
    }

    function closeMessage() {
        messageObj.close();
    }
    //POPUP DESABILITADO A PEDIDO DO GESTOR DO SISTEMA
    //montaShowModal();
    
</script>