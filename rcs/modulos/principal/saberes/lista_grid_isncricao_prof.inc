<?PHP
    if(isset($_POST['requisicao_excel'])){
        gerarNovoExcel();
    }
    
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }
    
    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Reconhecimento de Saberes e Competências', 'Processo Seletivo para Comissão Avaliadora' );
    
    $abacod_tela    = '';
    $url            = '';
    $parametros     = '';
    
    //$db->cria_aba($abacod_tela, $url, $parametros);
    
    $perfil = pegaPerfilGeral($_SESSION['usucpf']);
?>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">
    
    function cadastraProfessor(){
        window.location.href ='rcs.php?modulo=principal/saberes/cad_inscricao_prof&acao=A';
    }
    
    function editarServidor(srpid){
        window.location.href ='rcs.php?modulo=principal/saberes/cad_inscricao_prof&acao=A&srpid='+srpid;
    }
    
    function excluirServidor( srpid ){
        
        if(confirm('Deseja realmente excluir este professor?')){
            $('#srpid').val(srpid);
            $('#requisicao').val('excluirServidorProfessor');
            $('#formulario').submit();
        }
    }
    
    function pesquisarServidor(param){
        if(trim(param) == 'fil'){
            $('#formulario').submit();
        }else{
            $('#formulario').submit();
        }
    }

    function geraExcel(){        
        $('#requisicao_excel').val('gerarNovoExcel');
        $('#excel').submit();
    }
    
</script>
    

<form action="" method="POST" id="formulario" name="formulario">
    
<input type="hidden" id="srpid" name="srpid" value=""/>
<input type="hidden" id="requisicao" name="requisicao" value=""/>
    
    <table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">
        
        <tr>
            <td class ="SubTituloDireita" width="35%">SIAPE:</td>
            <td>
                <?= campo_texto('dsamatricula', 'N', 'S', '', 14, 10, '', '', '', '', 0, 'id="dsamatricula"', '', $dsamatricula, null, '', null); ?>
            </td>
        </tr>        
        <tr>
            <td class ="SubTituloDireita">CPF:</td>
            <td>
                <?= campo_texto('srpnumcpf', 'N', 'S', '', 14, 16, '###.###.###-##', '', '', '', 0, 'id="srpnumcpf"', '', $srpnumcpf, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Nome do Servidor:</td>
            <td>
                <?= campo_texto('srpdsc', 'N', 'S', '', 45, 200, '', '', '', '', 0, 'id="srpdsc"', '', $srpdsc, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Instituição (Sigla):</td>
            <td>
                <?= campo_texto('entsig', 'N', 'S', '', 45, 200, '', '', '', '', 0, 'id="entsig"', '', $entsig, null, '', null); ?>
            </td>
        </tr>
        <?PHP
            if( in_array(PERFIL_RCS_ADMINISTRADOR, $perfil) || in_array(PERFIL_RCS_SUPER_USUARIO, $perfil) ){
        ?>
        <tr>
            <td class ="SubTituloDireita">Status:</td>
            <td>
                 <?PHP
                    $sql = "
                        SELECT  'A' as codigo,
                                'Ativo' as descricao
                        UNION
                        SELECT  'I' as codigo,
                                'Inativo' as descricao
                    ";
                    $db->monta_combo('srpstatus', $sql, 'S', "Selecione...", '', '', '', 320, 'N', 'srpstatus', '', $srpstatus, 'Status', '', '');
                ?>
            </td>
        </tr>
        <?PHP
            }
        ?>
        <tr>
            <td class="SubTituloCentro" colspan="2"> 
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarServidor('fil');"/>
                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarServidor('ver');"/>
            </td>
        </tr>
    </table>
</form>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
    <tr>
        <td class="SubTituloEsquerda" colspan="4">Legenda</td>
    </tr>
    <tr>
        <td>
            <img border="0" align="absmiddle" src="../imagens/rcs/lampada_16x16.png" width="14" title="Professor Ativo"> Professor Ativo &nbsp;
            <br>
            <img border="0" align="absmiddle" src="../imagens/rcs/lamapda_of_16x16.png" width="14" title="Professor Ativo"> Professor Inativo &nbsp;
        </td>
        <td>
            <img border="0" align="absmiddle" src="../imagens/icones/bb.png" title="Existe uma banca montada para este professor, ele está sendo avalido" > Existe uma banca montada para este professor, ele está sendo avalido &nbsp;
            <br>
            <img border="0" align="absmiddle" src="../imagens/icones/br.png" title="Não existe uma banca montada para este professor, ele não está sendo avalido" > Não existe uma banca montada para este professor, ele não está sendo avalido &nbsp;
        </td>
        <td>
            <img border="0" align="absmiddle" src="../imagens/icones/bg.png" title="É um professor há ser Avaliado" > É um professor Avaliador e faz parte de uma Banca Avaliadora &nbsp;
            <br>
            <img border="0" align="absmiddle" src="../imagens/icones/by.png" title="É um professor Avaliador" > Não é um professor Avaliador não faz parte de uma Banca Avaliadora &nbsp;
        </td>
        <td><img border="0" align="absmiddle" src="../imagens/ico_ajuda.gif" title="Professor não definido"> Professor não definido &nbsp;
            <br>
            <img border="0" align="absmiddle" src="../imagens/check_p.gif" title="Deseja ser um Avaliador e/ou Avaliado"> Deseja ser um Avaliador e/ou Avaliado &nbsp;
            <br>
            <img border="0" align="absmiddle" src="../imagens/unchecked.jpg" title="Não deseja ser Avaliado e/ou Avaliado"> Não deseja ser Avaliador e/ou Avaliado &nbsp;
        </td>
    </tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td>
            <span style="cursor: pointer" onclick="cadastraProfessor();" title="Novo Professor" >
                <img align="absmiddle" src="/imagens/gif_inclui.gif" width="11px"/> &nbsp; Cadastrar Novo Professor
            </span>
        </td>
    </tr>    
</table>

<br>

<?PHP
    $perfil = pegaPerfilGeral($_SESSION['usucpf']);
    
    if( in_array(PERFIL_RCS_INTERLOCUTOR, $perfil) || in_array(PERFIL_RCS_ADM_INSTITUCIONAL, $perfil) ){
        $SELECT = "AND ent.entid IN (SELECT entid FROM rcs.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}' )";
    }else{
        $SELECT = "";
    }

    if ($_REQUEST['dsamatricula']){
        $where .= " AND s.dsamatricula = '{$_REQUEST['dsamatricula']}'";
    }
    if ($_REQUEST['srpnumcpf']) {
        $where .= " AND s.srpnumcpf = '".str_replace( '-', '', str_replace( '.', '', $_REQUEST['srpnumcpf'] ) )."'";
    }
    if ($_REQUEST['srpdsc']) {
        $srpdsc = addslashes($_REQUEST['srpdsc']);
        $where .= " AND public.removeacento(s.srpdsc) ilike public.removeacento( ('%{$srpdsc}%') )";
    }
    
    if ($_REQUEST['entsig']) {
        $where .= " AND public.removeacento(ent.entsig) ilike public.removeacento( ('%".trim($_REQUEST['entsig'])."%') )";
    }
    
    if ($_REQUEST['srpstatus']) {
        $where .= " AND s.srpstatus = '{$_REQUEST['srpstatus']}'";
    }
    
    $acao = "
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarServidor('||s.srpid||');\" title=\"Editar Professor\" >
    ";
    
    if( in_array(PERFIL_RCS_ADMINISTRADOR, $perfil) || in_array(PERFIL_RCS_ADM_INSTITUCIONAL, $perfil) || in_array(PERFIL_RCS_SUPER_USUARIO, $perfil)){
        $acao2 = "<img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirServidor('||s.srpid||');\" title=\"Excluir Professor\" >";
    } else {
        $acao2 = '';
    }
    
    $icone_at = '<img border="0" align="absmiddle" src="../imagens/rcs/lampada_16x16.png" width="14" title="Professor Ativo">';
    $icone_in = '<img border="0" align="absmiddle" src="../imagens/rcs/lamapda_of_16x16.png" width="14" title="Professor Ativo">';
    
    $icones_bb = '<img border="0" align="absmiddle" src="../imagens/icones/bb.png" title="Existe uma banca montada para este professor, ele está sendo avalido">';
    $icones_br = '<img border="0" align="absmiddle" src="../imagens/icones/br.png" title="Não existe uma banca montada para este professor, ele não está sendo avalido">';
    $icones_bg = '<img border="0" align="absmiddle" src="../imagens/icones/bg.png" title="É um professor Avaliador">';
    $icones_by = '<img border="0" align="absmiddle" src="../imagens/icones/by.png" title="Não é um professor Avaliador">';
    
    $icones_n_def = '<img border="0" align="absmiddle" src="../imagens/ico_ajuda.gif" title="Professor não definido">';
    $icones_av = '<img border="0" align="absmiddle" src="../imagens/check_p.gif" title="Deseja ser um Avaliador e/ou Avaliado">';
    $icones_n_av = '<img border="0" align="absmiddle" src="../imagens/unchecked.jpg" title="Não deseja ser Avaliado e/ou Avaliado">';
    
    $sql = "
        SELECT  DISTINCT '{$acao} {$acao2}' as acao,
                s.dsamatricula,
                replace(to_char(cast(s.srpnumcpf as bigint), '000:000:000-00'), ':', '.') as srpnumcpf,
                s.srpdsc,
                UPPER(ent.entsig) AS entsig,

                CASE WHEN a.srpid is not null 
                    THEN '{$icones_bb}'
                    ELSE '{$icones_br}'
                END AS srpid_avalido,

                CASE WHEN ar.srpid is not null 
                    THEN '{$icones_bg}'
                    ELSE '{$icones_by}'
                END AS srpid_avalidor,

                CASE WHEN srptipoavaliador IS NULL
                    THEN '{$icones_n_def}'
                    ELSE
                        CASE WHEN srptipoavaliador IS TRUE
                            THEN '{$icones_av}'
                            ELSE '{$icones_n_av}'
                        END                    
                END AS avaliador,
                CASE WHEN srptipoavaliado IS NULL
                    THEN '{$icones_n_def}'
                    ELSE 
                        CASE WHEN srptipoavaliado IS TRUE
                            THEN '{$icones_av}'
                            ELSE '{$icones_n_av}'
                        END 
                END as avaliado,

                s.srptdddelcelular ||'-'|| s.srptelcelular,

                 CASE WHEN srpstatus = 'A'
                    THEN '{$icone_at}'
                    ELSE '{$icone_in}'
                END AS srpstatus

        FROM rcs.servidoresprofessor AS s

        LEFT JOIN rcs.avaliado AS a ON a.srpid = s.srpid AND avastatus = 'A'

        LEFT JOIN rcs.avaliador AS ar ON ar.srpid = s.srpid  AND avrstatus = 'A'

        LEFT JOIN rcs.usuarioresponsabilidade AS u ON u.usucpf = s.srpnumcpf AND rpustatus = 'A'

        LEFT JOIN entidade.entidade AS ent ON ent.entid = s.srplotacao

        WHERE 1=1 {$where} {$SELECT}

        ORDER BY s.srpdsc
    ";
    $cabecalho = array("Ação", "SIAPE", "CPF", "Servidor", "Lotação","Sendo Avalido", "Sendo Avaliador", "Avaliador", "Avaliado", "Tel. Celular", "Status");  
    $alinhamento = Array('center', 'center', 'center', '', '','center','center','center','center','', 'center');
    $tamanho = Array('4%', '7%', '7%', '30%', '15%', '3%', '3%', '3%', '3%', '7%', '3%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

?>
<!--<br/>
<div style="width:98%; margin: 0 auto;" >
    <form method="POST" name="excel" id="excel">
        <input type="hidden" id="requisicao_excel" name="requisicao_excel" value=""/>
        <button id="submit" onclick="geraExcel();">Exportar XLS</button>
    </form>
</div>-->