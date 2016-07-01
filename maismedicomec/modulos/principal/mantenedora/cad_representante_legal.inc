<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Mais Médico - MEC', 'Cadastro da Mantenedora' );
    
    $abacod_tela    = ABA_CADASTRO_MANTENEDORA;
    $url            = 'maismedicomec.php?modulo=principal/mantenedora/cad_representante_legal&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

    if($srpid == ''){
        $srpid = $_REQUEST['srpid'];
    }

    #QUANDO O ACESSADO PELO USUÁRIO PROFESSOR.
    if($_REQUEST['acUsProf'] == 'S' ){
        $srpnumcpf = $_SESSION['usucpf'];
        $sql = "
            SELECT srpid FROM rcs.servidoresprofessor WHERE srpnumcpf = '{$srpnumcpf}';
        ";
        $srpid = $db->pegaUm($sql);
    }

    if( $srpid != '' ){
        $dados = buscarDadosProfessores( $srpid );

        if( $dados['dsatermoaceite01'] = 't' ){
            $checked_01 = 'checked="checked"';
        }else{
            $checked_01 = '';
        }
        if( $dados['dsatermoaceite02'] = 't' ){
            $checked_02 = 'checked="checked"';
        }else{
            $checked_02 = '';
        }
        if( $dados['dsatermoaceite03'] = 't' ){
            $checked_03 = 'checked="checked"';
        }else{
            $checked_03 = '';
        }
    }
?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>


<script type="text/javascript">

    function atualizaComboMunicipio( estuf ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboMunicipio&estuf="+estuf,
            async: false,
            success: function(resp){
                $('#td_combo_municipio').html(resp);
            }
        });
    }

    function buscarEndereceCEP( cep ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarEndereceCEP&cep="+cep,
            success: function(resp){
                var dados = $.parseJSON(resp);

                $('#srplogradouro').val(dados.logradouro);
                $('#srpbairro').val(dados.bairro);
                $('#estuf').val(dados.estado);
                $('#muncod').val(dados.muncod);
            }
        });
    }

    function salvarDadosProfessores(){
        var erro;
        var campos = '';
        var formulario = document.formulario;

        $.each($(".obrigatorio"), function(i, v){
            if( ($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox') ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else
                if( $(this).attr('type') == 'radio' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var radio_box = $('input:radio[name='+name+']:checked');

                    if(!radio_box.val()){
                        erro = 1;
                        if(value == 'S'){
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
            }else
                if( $(this).attr('type') == 'checkbox' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name='+name+']:checked');

                    if(!check_box.val()){
                        erro = 1;
                        if(value == 'S'){
                            campos += '- ' + $(this).attr('title') + " \n";
                        }
                    }
            }
        });

        selectAllOptions( formulario.grdid );
        selectAllOptions( formulario.espid );
        selectAllOptions( formulario.mesid );
        selectAllOptions( formulario.douid );

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarDadosProfessores');
            $('#formulario').submit();
        }
    }

    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'rcs.php?modulo=principal/inicio_direcionamento&acao=A';
        janela.focus();
    }

</script>

<?PHP
    $perfil = pegaPerfilGeral($_SESSION['usucpf']);
    
    if( in_array(PERFIL_RCS_PROFESSOR, $perfil) ){
        $cpf = $_SESSION['usucpf'];
    }else{
        $cpf = $dados['srpnumcpf'];        
    }
    if( $cpf != '' ){
       buscaUnidadesAssociadas( $cpf );
    }
?>


<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="srpid" name="srpid" value="<?=$srpid?>"/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Identificação </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> CPF: </td>
            <td colspan="2">
                <?PHP
                    $mntcnpj = $dados['mntcnpj'];
                    echo campo_texto('mntcnpj', 'S', 'S', 'CPF', 29, 18, '###.###.###-##', '', '', '', 0, 'id="mntcnpj"', '', $srpnumcpf, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Nome: </td>
            <td colspan="2">
                <?PHP
                    $rpldsc = $dados['rpldsc'];
                    echo campo_texto('rpldsc', 'S', 'S', 'Nome', 59, 100, '', '', '', '', 0, 'id="rpldsc"', '', $rpldsc, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Sexo: </td>
            <td colspan="2">
                <input type="radio" name="rplsexo" id="rplsexo_m" value="M" class="obrigatorio"> Masculino
                <input type="radio" name="rplsexo" id="rplsexo_f" value="F" class="obrigatorio"> Feminico
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> RG: </td>
            <td colspan="2">
               <?PHP
                    $rplrg = $dados['rplrg'];
                    echo campo_texto('rplrg', 'S', 'S', 'RG', 29, 20, '####################', '', 'left', '', 0, 'id="rplrg"', '', $rplrg, '', '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Órgão Expeditor: </td>
            <td colspan="2">
               <?PHP
                    $rplorgaoexprg = $dados['rplorgaoexprg'];
                    echo campo_texto('rplorgaoexprg', 'S', 'S', 'Órgão Expeditor', 59, 10, '', '', 'left', '', 0, 'id="rplorgaoexprg"', '', $rplorgaoexprg, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> UF: </td>
            <td colspan="2">
                <?PHP
                    $estuf = $dados['estuf'];
                    $sql = "
                         SELECT  estuf as codigo,
                                 estuf as descricao
                         FROM territorios.estado
                         ORDER BY estuf
                     ";
                     $db->monta_combo('estuf', $sql, 'S', "Selecione...", 'atualizaComboMunicipio', '', '', 210, 'S', 'estuf', '', $estuf, 'UF');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone Comercial: </td>
            <td colspan="2">
               <?PHP
                    $textoDica = 'O telefone é composto por código de área (DDD) - Prefixo - Número do telefone. Ex: 61-2200-1111.';
                    
                    $rpltelcomercial = $dados['rpltelcomercial'];
                    echo campo_texto('rpltelcomercial', 'N', 'S', 'Telefone Comercial', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="rpltelcomercial"', '', $rpltelcomercial, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita"> Telefone Celular: </td>
            <td colspan="2">
               <?PHP
                    $rpltelcelular = $dados['rpltelcelular'];
                    echo campo_texto('rpltelcelular', 'N', 'S', 'Telefone Celular', 29, 12, '##-####-####', '', 'left', $textoDica, 0, 'id="rpltelcelular"', '', $rpltelcelular, null, '', null);
                ?>                
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> E-mail Institucional: </td>
            <td colspan="2">
                <?PHP
                    $rplemail = $dados['rplemail'];
                    echo campo_texto('rplemail', 'N', 'S', 'E-mail', 59, 100, '', '', '', '', 0, 'id="rplemail"', '', $rplemail, '', null);
                ?>
            </td>
        </tr>
    </table>
    
    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
                <input type="button" id="salvar" name="salvar" value="Salvar" onclick="salvarDadosProfessores();"/>
                <input type="button" id="cancelar" name="cancelar" value="Cancelar" onclick="voltarPaginaPrincipal();"/>
            </td>
        </tr>
    </table>
</form>

<br>

<?PHP
    /*
SELECT mtdid, estuf, usucpf, muncod, mtddsc, mntsigla, mtdimovelsit, 
       mtdlogradouro, mtdcomplementoend, mtdbairro, mtdtelefone01, mtdtelefone02, 
       mtdtelefone03, mtdsite, mtdfilantropica, mtdcomunitaria, mtdconfessional, 
       mtdautfuncionamento, mtdcredenciamento, mtdstatus, mtddtinclusao
  FROM maismedicomec.mantida;

*/
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
        <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarServidor('||s.srpid||');\" title=\"Editar Professor\" >
    ";
    
    $sql = "
        SELECT  rpldsc, 
                rplcpf, 
                rplsexo, 
                rplrg, 
                rplorgaoexprg, 
                estuf,
                rpltelcomercial, 
                rpltelcelular, 
                rplemail
  FROM maismedicomec.representantelegal;

    ";
    $cabecalho = array("Ação", "Nome", "CPF", "Sexo", "RG", "Órgão Expeditor", "UF", "Tel. Comercial", "Tel. Celular", "E-mail");  
    $alinhamento = Array('center', '', '', '', '','','','','');
    $tamanho = Array('2%', '', '', '', '', '','','','');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

?>
