<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Reconhecimento de Saberes e Competências', 'Processo Seletivo para Banca Avaliadora' );

    $abacod_tela    = ABA_QUESTIONARIO_MEDICINA;
    $url            = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_aval_documental&acao=A';
    $parametros     = '';
    //$db->cria_aba($abacod_tela, $url, $parametros);

    $perfis = pegaPerfilGeral($_SESSION['usucpf']);

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
    }
    
    if( !$_POST['pesquisar'] ){
        $exibirFormularioPesquisa = 'style="display: none;"';
    }
?>

<link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css"></link>
<link rel="stylesheet" type="text/css" href="../rcs/css/modal_css_basic.css"/>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<script type="text/javascript" src="../rcs/javascript/jquery.simplemodal.js"></script>


<div id="dialog_confirm"></div>

<div id="div_pai_modal_bancada" style="background-color: slategrey; padding:2px; -moz-border-radius:5px; -webkit-border-radius:5px; border-radius:8px; width: 98%; height: 92%; display: none;"> 
    <div id="div_modal_bancada" style="margin-top: 2%;"> </div>
</div>

<script type="text/javascript">
    
    function abrirPesquisa(opc){
        if(opc == 'mais'){
            $('#sinal_mais').css("display", "none");
            $('#sinal_menos').css("display", "");
            $('.containerFiltroFormulario').fadeIn();
            $('.containerFiltro').show();
        }else{
            $('#sinal_mais').css("display", "");
            $('#sinal_menos').css("display", "none");
            $('.containerFiltro').fadeIn();
            $('.containerFiltroFormulario').hide('fast');
        }
    }

    function buscarDadosCPF( srpnumcpf ){
        if( srpnumcpf != '' ){
            $.ajax({
                type    : "POST",
                url     : window.location.href,
                data    : "requisicao=buscarDadosPorCPF_ParaMontagemBanca&srpnumcpf="+srpnumcpf,
                success: function(resp){
                    resetFromulario();
                    var dados = $.parseJSON(resp);

                    if( dados.usu_cpf_igual == 'N' ){
                        if(trim(dados.dsamatricula) != ''){
                            if( dados.prof_equipe == 'N' ){
                                alert('Esse professor não faz parte do Institudo de sua responsabilidade. Verifica os dados digitados!');
                            }else{
                                $('#srpid').val(dados.srpid);
                                $('#srplotacao').val(dados.srplotacao);
                                $('#srpdsc').val(dados.srpdsc);
                                $('#dsamatricula').val(dados.dsamatricula);
                                $('#srpnumcpf').val(dados.srpnumcpf);
                            }
                        }else{
                             if( confirm('SIAPE e/ou CPF Não consta em nossa base de dados é necessário cadastrá-lo! Deseja cadastrá-lo agora?') ){
                                window.location.href = '/rcs/rcs.php?modulo=principal/saberes/cad_inscricao_prof&acao=A'
                            }
                        }
                    }else{
                        alert('Não é possivél realizar a propriá Banca Avaliadora');
                    }
                }
            });
        }
    }

    function excluirAvaliador( avrid , element){
        var confirma = confirm('Deseja excluir este avaliador da bancada?');
        if( confirma ){
            $1_11.post(
                window.location.href , {
                    requisicao: 'excluirAvaliador' ,
                    avrid: avrid
                },
                function( html ){
                    alert(html);
                    $(element).closest('tr').fadeOut();
                }
            );
        }
    }

    function excluirBancaAvaliadora( avaid ){
        var confirma = confirm("Tem certeza que deseja excluri essa Banca Avaliadora?");

        if( confirma ){
            $('#avaid').val(avaid);
            $('#requisicao').val('excluirBancaAvaliadora');
            $('#formulario').submit();
        }
    }

    function formularioBancaAvaliadora(avaid){
        
         $.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=formularioBancaAvaliadora&avaid="+avaid,
            success: function (resp){
                $('#div_modal_bancada').html(resp);
                $('#div_pai_modal_bancada').modal();
                divCarregado();
            }
        });
    }

    function salvarDadosAvaliador(){
        var erro;
        var campos = '';

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

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarDadosAvaliador');
            $('#formulario').submit();
        }
    }

    function resetFromulario(){
        $.each($("input[type=text], input[type=hidden], select, textarea, input[type=radio], input[type=checkbox]"), function(i,v){
            if( $(this).attr('type') != 'radio' && $(this).attr('type') != 'checkbox'){
                $(v).val('');
            }
            if( $(this).attr('type') == 'radio' ){
                $(v).attr('checked', false);
            }
            if( $(this).attr('type') == 'checkbox' ){
                $(v).attr('checked', false);
            }
        });
    }

    function pesquisarAvaladores(opc){
        var formulario = $('#form_pesquisa');
        if(opc == "p"){
            formulario.closest('form[name=form_pesquisa]').submit();
        }else{
             window.location.href = window.location.href;
        }
    }

    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'rcs.php?modulo=principal/inicio_direcionamento&acao=A';
        janela.focus();
    }
    
    function verificaSeExisteBancaNumProcesso( numprocesso ){
        var dsamatricula = $('#dsamatricula').val();
        
        if( dsamatricula != '' && numprocesso != '' ){            
            $.ajax({
                type    : "POST",
                url     : window.location.href,
                data    : "requisicao=verificaSeExisteBancaNumProcesso&dsamatricula="+dsamatricula+"&numprocesso="+numprocesso,
                success: function(resp){
                    var dados = $.parseJSON(resp);
                    if( dados.existe_banca_nprocesso == 'S' ){
                        $('#avanumprocesso').val('');
                        $('#btSalvar').attr('disabled', true);
                        alert('Esse professor está com a Banca Avaliadora já montada para esse número de processo. So é possível montar nova banca com um novo processo!');
                    }else{
                        $('#btSalvar').attr('disabled', false);
                    }
                }
            });
        }
    }

    //#CARREGAR LISTA FILHO
    function exibirListaAvaliadores(avaid, td_nome){
	divCarregando();
	$.ajax({
            type    : "POST",
            url     : window.location.href,
            data    : "requisicao=exibirListaAvaliadores&avaid="+avaid,
            async: false,
            success: function(msg){
                $('#'+td_nome).html(msg);
                divCarregado();
            }
	});
    }

    //#CARREGAR LISTA FILHO
    function carregarListaTarefa(idImg, avaid){
        var img     = $( '#'+idImg );
        var tr_nome = 'listaAvaliadores_'+ avaid;
        var td_nome = 'trA_'+ avaid;

        if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == "" ){
            $('#'+td_nome).html('Carregando...');
            img.attr ('src','../imagens/menos.gif');
            exibirListaAvaliadores(avaid, td_nome);
        }
        if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
            $('#'+tr_nome).css('display','');
            img.attr('src','../imagens/menos.gif');
        } else {
            $('#'+tr_nome).css('display','none');
            img.attr('src','/imagens/mais.gif');
        }
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="orgid" name="orgid" value=""/>
    <input type="hidden" id="avaid" name="avaid" value=""/>
    <input type="hidden" id="srpid" name="srpid" value="<?=$srpid?>"/>
    <input type="hidden" id="srplotacao" name="srplotacao" value=""/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Dados Pessoais </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> CPF: </td>
            <td colspan="2">
                <?PHP
                    $srpnumcpf = $dados['srpnumcpf'];
                    echo campo_texto('srpnumcpf', 'S', $habilita, 'CPF', 25, 14, '###.###.###-##', '', '', '', 0, 'id="srpnumcpf"', '', $srpnumcpf, 'buscarDadosCPF(this.value)', null);
                ?>
                <input onclick="javascript:window.location.href = '/rcs/rcs.php?modulo=principal/saberes/cad_inscricao_prof&acao=A'" type="button" value="Cadastrar Professor" />
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Matricula SIAPE: </td>
            <td colspan="2">
                <?PHP
                    $dsamatricula = $dados['dsamatricula'];
                    echo campo_texto('dsamatricula', 'N', 'N', 'Matricula SIAPE', 25, 7, '', '', '', '', 0, 'id="dsamatricula"', '', $dsamatricula, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Candidato: </td>
            <td colspan="2">
                <?PHP
                    $srpdsc = $dados['srpdsc'];
                    echo campo_texto('srpdsc', 'N', 'N', 'Candidato', 60, 100, '', '', '', '', 0, 'id="srpdsc"', '', $srpdsc, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Nº do Processo de Solicitação: </td>
            <td colspan="2">
                <?PHP
                    $avanumprocesso = $dados['avanumprocesso'];
                    echo campo_texto('avanumprocesso', 'S', $habilita, 'Nº do Processo de Solicitação', 25, 20, '####################', '', '', '', 0, 'id="avanumprocesso"', '', $avanumprocesso, 'verificaSeExisteBancaNumProcesso(this.value)', null);
                ?>
            </td>
        </tr>

        <tr>
            <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Formação da Banca Avaliadora </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="25%"> Avaliadores Externos: </td>
            <td colspan="2">
                <?PHP
                    $avaliador_aex = $dados['avaliador_aex'];
                    echo campo_texto('avaliador_aex', 'S', $habilita, 'Avaliadores Externos', 5, 2, '', '', '', '', 0, 'id="avaliador_aex"', '', $avaliador_aex, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Avaliadores Internos: </td>
            <td colspan="2">
                <?PHP
                    $avaliador_ain = $dados['avaliador_ain'];
                    echo campo_texto('avaliador_ain', 'N', $habilita, 'Avaliadores Internos', 5, 2, '', '', '', '', 0, 'id="avaliador_ain"', '', $avaliador_ain, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Avaliadores Externos Suplentes: </td>
            <td colspan="2">
                <?PHP
                    $avaliador_aes = $dados['avaliador_aes'];
                    echo campo_texto('avaliador_aes', 'N', $habilita, 'Avaliadores Externos Suplentes', 5, 2, '', '', '', '', 0, 'id="avaliador_aes"', '', $avaliador_aes, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Avaliadores Internos Suplentes: </td>
            <td colspan="2">
                <?PHP
                    $avaliador_ais = $dados['avaliador_ais'];
                    echo campo_texto('avaliador_ais', 'N', $habilita, 'Avaliadores Internos Suplentes', 5, 2, '', '', '', '', 0, 'id="avaliador_ais"', '', $avaliador_ais, '', null);
                ?>
            </td>
        </tr>
    </table>
    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td class="SubTituloCentro" colspan="4" style="font-weight: bold;">
                <?PHP
                        $habilita = 'S';
                    if( $habilita == 'S' ){
                ?>
                        <input type="button" id="btSalvar" name="btSalvar" value="Salvar" onclick="salvarDadosAvaliador();"/>
                        <input type="button" id="btCancelar" name="btCancelar" value="Cancelar" onclick="voltarPaginaPrincipal();"/>
                <?PHP
                    }else{
                ?>
                        <input type="button" id="salvar" name="salvar" value="Salvar" disabled="disabled"/>
                        <input type="button" id="cancelar" name="cancelar" value="Cancelar"disabled="disabled"/>
                <?PHP
                    }
                ?>
            </td>
        </tr>
    </table>
</form>


<!-------------------------------------------------------------- FORMULÁRIO DE PRESQUISA --------------------------------------------------------------->

<form id="form_pesquisa" name="form_pesquisa" method="post">
    <input type="hidden" name="pesquisar" value="true" />
    
    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
        <tr>
            <td colspan="2" class="subTituloCentro" style="text-align: left;">
                <img id="sinal_mais"  style="cursor: pointer; " src="../imagens/mais.gif" onclick="abrirPesquisa('mais');">
                <img id="sinal_menos" style="display: none; cursor: pointer;" src="../imagens/menos.gif" onclick="abrirPesquisa('menos');">
                <span style="margin-left: 10px; text-transform:uppercase;"> Pesquisar avaliados/banca avaliadora </span>
            </td>
        </tr>        
        <tr class="containerFiltroFormulario"  <?php echo $exibirFormularioPesquisa ?>>
            <td class="subTituloCentro" colspan="2" style="text-align: center; text-transform:uppercase;"> Formulário para listagem dos avaliados/banca avaliadora </td>
        </tr>
        <tr class="containerFiltroFormulario" <?php echo $exibirFormularioPesquisa ?>>
            <td class="SubTituloDireita" width="35%"> Professor: </td>
            <td colspan="2">
                <?PHP
                    $srpdsc = $dados['srpdsc'];
                    echo campo_texto('srpdsc', 'N', 'S', 'Candidato', 70, 100, '', '', '', '', 0, 'id="srpdsc"', '', $srpdsc, '', null);
                ?>
            </td>
        </tr>
        <tr class="containerFiltroFormulario" <?php echo $exibirFormularioPesquisa ?>>
            <td class="SubTituloDireita" width="25%"> SIAPE: </td>
            <td colspan="2">
                <?PHP
                    $dsamatricula = $dados['dsamatricula'];
                    echo campo_texto('dsamatricula', 'N', 'S', 'Matricula SIAPE', 35, 7, '', '', '', '', 0, 'id="dsamatricula"', '', $dsamatricula, '', null);
                ?>
            </td>
        </tr>
        <tr class="containerFiltroFormulario" <?php echo $exibirFormularioPesquisa ?>>
            <td class="SubTituloDireita" width="25%"> CPF: </td>
            <td colspan="2">
                <?PHP
                    $srpnumcpf = $dados['srpnumcpf'];
                    echo campo_texto('srpnumcpf', 'N', 'S', 'CPF', 35, 14, '###.###.###-##', '', '', '', 0, 'id="srpnumcpf"', '', $srpnumcpf, '', null);
                ?>
            </td>
        </tr>
        <tr class="containerFiltroFormulario" <?php echo $exibirFormularioPesquisa ?>>
            <td class="SubTituloDireita" width="25%">Processo: </td>
            <td colspan="2">
                <?PHP
                    $avanumprocesso = $dados['avanumprocesso'];
                    echo campo_texto('avanumprocesso', 'N', 'S', 'Nº do Processo de Solicitação', 35, 20, '####################', '', '', '', 0, 'id="avanumprocesso"', '', $avanumprocesso, '', null);
                ?>
            </td>
        </tr>
        <tr class="containerFiltroFormulario" <?php echo $exibirFormularioPesquisa ?>>
            <td class="SubTituloDireita" width="25%">Lotação: </td>
            <td colspan="2">
                <?PHP
                    $entnome = $dados['entnome'];
                    echo campo_texto('entnome', 'N', 'S', '', 35, 20, '' , '', '', '', 0, 'id="entnome"', '', $entnome, '', null);
                ?>
            </td>
        </tr>
        <tr class="containerFiltroFormulario" <?php echo $exibirFormularioPesquisa ?>>
            <td class="SubTituloDireita" width="25%">Período do Sorteio: </td>
            <td colspan="2">
                <?PHP
                    $avadatasorteioIni = $dados['avadatasorteio_ini'];
                    echo campo_data2('avadatasorteio_ini','N','S','','DD/MM/YYYY','','', $avadatasorteio_ini, '', '', 'avadatasorteio_ini' );
                    echo " até ";
                    $avadatasorteio_fim = $dados['avadatasorteio_fim'];
                    echo campo_data2('avadatasorteio_fim','N','S','','DD/MM/YYYY','','', $avadatasorteio_fim, '', '', 'avadatasorteio_fim' );                
                ?>
            </td>
        </tr>
        <tr class="containerFiltroFormulario" <?php echo $exibirFormularioPesquisa ?>>
            <td colspan="2" style="text-align: center;">
                <input id="pesquisa" type="button" value="Pesquisar" onclick="pesquisarAvaladores('p', this)" />
                <input id="cancelar" type="button" value="Cancelar" onclick="pesquisarAvaladores('c', this)"/>
            </td>
        </tr>
    </table>
</form>

<?PHP

    if( in_array(PERFIL_RCS_INTERLOCUTOR, $perfis) ){
        $WHERE = "WHERE CAST(s.srplotacao AS integer) IN (SELECT entid FROM rcs.usuarioresponsabilidade WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}')";
    }else{
        $WHERE = "";
    }

    if( $_POST['pesquisar'] ){
        $exibirFormularioPesquisa = '';
        $exibirBotaoPesquisa = 'style="display: none;"';

        $arrWhere = array(
                's.dsamatricula' => $_POST['dsamatricula'],
                's.srpnumcpf' => str_replace(array('-' , '.') , '', $_POST['srpnumcpf']),
                's.srpdsc' => $_POST['srpdsc'],
                'a.avanumprocesso' => $_POST['avanumprocesso'],
                'e.entnome' => $_POST['entnome']
                );

        foreach($arrWhere as $keyWhere => $valueWhere){
            if(!empty($valueWhere)){
                if(empty($WHERE)){
                    $WHERE .= " WHERE {$keyWhere} ILIKE '%{$valueWhere}%' ";
                } else {
                    $WHERE .= " AND {$keyWhere} ILIKE '%{$valueWhere}%' ";
                }
            }
        }

        $whereOrEnd = (empty($WHERE))? ' WHERE ' : ' AND ';
        if(!empty($_POST['avadatasorteio_ini']) && !empty($_POST['avadatasorteio_fim']) ){
            $WHERE .= "{$whereOrEnd} avadatasorteio BETWEEN TO_DATE('{$_POST['avadatasorteio_ini']}' , 'DD/MM/YYYY') AND TO_DATE('{$_POST['avadatasorteio_fim']}' , 'DD/MM/YYYY') ";
        }
        
        $dados = $_POST;
    }

?>

<table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela">
    <tr>
        <td class="subTituloCentro" colspan="3" style="text-align: center; text-transform:uppercase;"> Listagem dos Avalidos/Banca Avaliadora </td>
    </tr>
</table>

<?PHP
    $perfis = pegaPerfilGeral();
    
    if( in_array(PERFIL_RCS_SUPER_USUARIO, $perfis) || in_array(PERFIL_RCS_ADMINISTRADOR, $perfis) || in_array(PERFIL_RCS_INTERLOCUTOR, $perfis) ){
        $acao = "
            <img align=\"absmiddle\" src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluirBancaAvaliadora('||a.avaid||');\" title=\"Excluir Avaliação\" >
            <img align=\"absmiddle\" src=\"/imagens/gif_inclui.gif\" style=\"cursor: pointer\" onclick=\"formularioBancaAvaliadora('||a.avaid||');\" title=\"Incluir Avaliadores\" >
            <img id=\"img_dimensao_' || a.avaid || '\" src=\"/imagens/mais.gif\" style=\"cursor: pointer\" onclick=\"carregarListaTarefa(this.id,\'' || a.avaid || '\');\"/>
        ";
    }else{
        $acao = "
            <img align=\"absmiddle\" src=\"/imagens/gif_inclui.gif\" style=\"cursor: pointer\" onclick=\"formularioBancaAvaliadora('||a.avaid||');\" title=\"Incluir Avaliadores\" >
            <img id=\"img_dimensao_' || a.avaid || '\" src=\"/imagens/mais.gif\" style=\"cursor: pointer\" onclick=\"carregarListaTarefa(this.id,\'' || a.avaid || '\');\"/>
        ";        
    }

    $sql = "
        SELECT  DISTINCT '{$acao}',
                s.srpdsc,
                CASE WHEN srptipoavaliador IS TRUE THEN 'Sim' ELSE 'Não' END AS avaliador,
                CASE WHEN srptipoavaliado IS TRUE THEN 'Sim' ELSE 'Não' END  as avaliadoo,
                ' ' || s.dsamatricula || ' ' AS dsamatricula,
                TRIM( replace(to_char(cast(s.srpnumcpf as bigint), '000:000:000-00'), ':', '.') ) as usucpf,
                ' ' || a.avanumprocesso || ' ' AS avanumprocesso,
                e.entnome ,
                s.srpemail,
                s.srptelcelular,
                TO_CHAR(avadatasorteio, 'DD/MM/YYYY') AS avadatasorteio,
                '<tr style=\"display:none;\" id=\"listaAvaliadores_' || a.avaid || '\" ><td></td><td id=\"trA_' || a.avaid || '\" colspan=\"11\"></td></tr>' as listaAvaliadores
        FROM rcs.avaliado AS a
        JOIN rcs.servidoresprofessor AS s ON s.srpid = a.srpid
        LEFT JOIN rcs.usuarioresponsabilidade AS u ON u.usucpf = s.usucpf
        LEFT JOIN entidade.entidade AS e ON e.entid = u.entid
        {$WHERE}
        ORDER BY S.srpdsc
    ";
    $cabecalho = array("Ação", "Professor" , 'Avaliador' , 'Avaliado' , "SIAPE", "CPF", "Processo" , 'Lotação' , "E-mail", "Celular", "Data do Sorteio", "");
    $alinhamento = Array('center', '', '', '', '','', '', 'center');
    $tamanho = Array('4%' , '20%', '5%', '3%', '8%', '8%', '8%' , '25%' , '15%', '7%', '7%', '1%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
?>