<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']();
    }
    
    if($_SESSION['maismedicomec']['mntid'] == ''){
        $db->sucesso("principal/mantenedora/cad_mantenedora", '', "Ocorreu um problema, é necessário o preenchimento dos dados da Mantida. Tente novamente!");
    }
    
    verificaMantenedora();
    
    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Mais Médicos - MEC', 'Cadastro de Hospital' );

  

    $abacod_tela    = ABA_CADASTRO_MANTENEDORA;
    $url            = 'maismedicomec.php?modulo=principal/mantenedora/lista_grid_hospital&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

    
  

?>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

    function atualizaComboMunicipioMantenedora( estuf ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboMunicipioMantenedora&estuf="+estuf,
            async: false,
            success: function(resp){
                $('#td_combo_municipio').html(resp);
            }
        });
    }

    function cadastraHospital(){
        window.location.href ='maismedicomec.php?modulo=principal/mantenedora/cad_hospital&acao=A';
    }

    function editarMantenedora(hspid){
        window.location.href ='maismedicomec.php?modulo=principal/mantenedora/cad_hospital&acao=A&hspid='+hspid;
    }

    function pesquisarServidor(param){
        if(trim(param) == 'fil'){
            $('#formulario').submit();
        }else{
            $('#formulario').submit();
        }
    }

</script>

<?php $mntid = $_SESSION['maismedicomec']['mntid'];
$sql = "select * from maismedicomec.mantenedora where mntid = ".$mntid;
$dados_mantenedora = $db->carregar($sql);

?>
   <table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">

        <tr>
            <td class ="SubTituloDireita" width="35%">Código mantenedora: </td>
            <td><?php echo $mntid ?></td>
        </tr>
        
          <tr>
            <td class ="SubTituloDireita" width="35%">Razão social</td>
            <td><?php echo $dados_mantenedora[0][mntrazaosocial];?></td>
        </tr>
   </table>

<!--

<form action="" method="POST" id="formulario" name="formulario">

<input type="hidden" id="requisicao" name="requisicao" value=""/>

    <table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">

        <tr>
            <td class ="SubTituloDireita" width="35%">Hospital:</td>
            <td>
                <?= campo_texto('hsprazaosocial', 'N', 'S', '', 89, 100, '', '', '', '', 0, 'id="mntrazaosocial"', '', $mntrazaosocial, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Sigla:</td>
            <td>
                <?= campo_texto('hspsigla', 'N', 'S', '', 49, 20, '', '', '', '', 0, 'id="mntsigla"', '', $mntsigla, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">CNPJ:</td>
            <td>
                <?= campo_texto('hspcnpj', 'N', 'S', '', 49, 14, '', '', '', '', 0, 'id="mntcnpj"', '', $mntcnpj, null, '', null); ?>
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
                    $db->monta_combo("estuf", $sql, 'S', 'Selecione...', 'atualizaComboMunicipioMantenedora', '', '', 300, 'N', 'estuf', false, $estuf, null);
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
                    $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', 300, 'N', 'muncod', false, $muncod, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarServidor('fil');"/>
                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarServidor('ver');"/>
            </td>
        </tr>
    </table>
</form>

-->

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td>
            <span style="cursor: pointer" onclick="cadastraHospital();" title="Novo Hospital" >
                <img align="absmiddle" src="/imagens/gif_inclui.gif" width="11px"/> &nbsp; Cadastrar Novo Hospital
            </span>
        </td>
    </tr>
</table>

<br>

<?PHP

    if ($_REQUEST['dsamatricula']) {
        $where .= " AND s.dsamatricula = '{$_REQUEST['dsamatricula']}'";
    }
    if ($_REQUEST['srpnumcpf']) {
        $where .= " AND s.srpnumcpf = '".str_replace( '-', '', str_replace( '.', '', $_REQUEST['srpnumcpf'] ) )."'";
    }
    if ($_REQUEST['srpdsc']) {
        $where .= " AND public.removeacento(s.srpdsc) ilike public.removeacento( ('%".trim($_REQUEST['srpdsc'])."%') )";
    }

    $acao = "
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarMantenedora('||m.hspid||');\" title=\"Editar Professor\" >
    ";

    // ver($_SESSION,d);
    // $_SESSION['maismedicomec']['mntid']
    $sql = "
        SELECT  '{$acao}' as acao,
                m.hsprazaosocial,
                m.hspsigla,
                m.hspcnpj,
                m.estuf,
                m.hsptelefone01,
                m.hspemail
        FROM maismedicomec.hospital AS m
        
        INNER JOIN maismedicomec.mantida manti ON manti.mtdid = m.mtdid
        INNER JOIN gestaodocumentos.instituicaoensino ie ON ie.iesid = m.mtdid

        INNER JOIN maismedicomec.mantenedora mante ON mante.mntid = ie.mntid AND mante.mntid = {$_SESSION['maismedicomec']['mntid']}
        INNER JOIN gestaodocumentos.mantenedoras mantes ON mantes.mntid = mante.mntid

        --WHERE srpstatus = 'A' {$where}

        ORDER BY m.hsprazaosocial
    ";
    // ver($sql,d);
    $cabecalho = array("Ação", "Razão Social", "Sigla", "CNPJ", "UF",'Telefone', 'E-mail');
    $alinhamento = Array('center','','','','','','','','');
    $tamanho = Array('2%','','','','','','','');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

?>