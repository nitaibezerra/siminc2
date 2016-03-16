<?PHP

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
}


include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Mais Médicos - MEC', 'Relatório de Mantenedoras');

$abacod_tela = '';
$url = '';
$parametros = '';

//$db->cria_aba($abacod_tela, $url, $parametros);


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

    function pesquisarServidor(param) {
        if (trim(param) == 'fil') {
        	$('#submite').val('OK');
        	$('#tipoxls').val('');
            $('#formulario').submit();
        } else {
           location.href = 'maismedicomec.php?modulo=relatorio/relatorio_mantenedora&acao=A';
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
	if ($_POST['mntrazaosocial']) {
	    $where .= " AND public.removeacento(m.mntdsc) ilike public.removeacento( ('%" . trim($_POST['mntrazaosocial']) . "%') )";
	}
	if ($_POST['mntsigla']) {
	    $where .= " AND public.removeacento(mm.mntsigla) ilike public.removeacento( ('%" . trim($_POST['mntsigla']) . "%') )";
	}
	if ($_POST['srpnumcpf']) {
	    $where .= " AND m.mntcnpj = '" . str_replace('.', '', str_replace('/', '', str_replace('-', '', $_POST['mntcnpj']))) . "'";
	}
	if ($_POST['estuf']) {
	    $where .= " AND mm.estuf = '" . $_POST['estuf'] . "'";
	}
	if ($_POST['muncod']) {
	    $where .= " AND mm.muncod = '" . $_POST['muncod'] . "'";
	}
	if ($_POST['situacao']) {
		if ($_POST['situacao'] == '1') {
	    	$where .= " AND doc.docid is null ";
		}else{
	    	$where .= " AND doc.esdid in (".$_POST['situacao'].") ";
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



$sql = "
		SELECT distinct

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
		--LEFT JOIN seguranca.perfilusuario pu on pu.usucpf = mm.usucpf $andPerfil
		--LEFT JOIN seguranca.perfil pe on pe.pflcod = pu.pflcod and pe.sisid = 185
		LEFT JOIN territorios.municipio AS mu ON  mu.muncod = mm.muncod
		$joinIES
		WHERE 1 = 1 {$where}
		ORDER BY 2
";

//ver($sql,d);

$cabecalho = array("Razão Social", "Sigla", "CNPJ", "UF", "Municípo", 'Telefone', 'E-mail', 'Situação');
$alinhamento = Array('center', '', '', '', '', '', '', '', '');
$tamanho = Array('2%', '', '', '', '', '', '', '');


if($_POST['tipoxls']=='OK'){
	ob_clean();
	header("Content-type: application/vnd.ms-excel");   
	header("Content-type: application/force-download");  
	header("Content-Disposition: attachment; filename=simec_maismedicosmec_mantenedora.xls");
	header("Pragma: no-cache");
	$db->monta_lista($sql, $cabecalho, 200, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
}else{
	$db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
}
?>