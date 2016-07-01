<?PHP

    function alerterarPerguntaQuest($dados){
        global $db;

        $prgid = $dados['prgid'];

        $sql = "
            SELECT  qstid,
                    prgid,
                    TRIM(prgdsc) AS prgdsc,
                    prgtipo,
                    prgtamanho,
                    prgobrigatoriedade,
                    TRIM(prgmascara) AS prgmascara,
                    prgiditem,
                    TRIM(prgmodalidade) AS prgmodalidade,
                    prgadicionaarq,
                    TRIM(TO_CHAR(prgdtinclusao, 'dd/mm/YYYY')) AS prgdtinclusao,
                    prgstatus
            FROM maismedicomec.pergunta AS p

            WHERE p.prgid = {$prgid}
        ";
        $dados = $db->carregar($sql);

        if($dados != ''){
            foreach ($dados as $i => $dado) {
                $dados[$i]['prgdsc'] = iconv( "ISO-8859-1", "UTF-8", stripslashes( $dado["prgdsc"] ) );
                $dados[$i]['prgmascara'] = iconv( "ISO-8859-1", "UTF-8", stripslashes( $dado["prgmascara"] ) );
            }
        }

        if($dados != ''){
            echo simec_json_encode( $dados );
        }
        die();
    }

    function excluirPerguntaQuest( $dados ) {
	global $db;

        $prgid = $dados['prgid'];
        $qstid = $dados['qstid'];

	if($prgid != '') {
            $sql = " DELETE FROM maismedicomec.pergunta WHERE prgid = {$prgid} RETURNING prgid; ";
	}
        $prgid = $db->pegaUm($sql);

        if($prgid){
            $db->commit();
            $parametros = "&qstid=".$qstid;
            $db->sucesso('sistema/tabela_apoio/cad_monta_perguntas', $parametros, 'Pergunta excluida com sucesso!', 'N', 'N');
        } else {
            $db->insucesso('Não foi possível realizar a operação.', '', 'sistema/tabela_apoio/cad_monta_perguntas&acao=A');
        }
    }

    function listagemPerguntaQuest( $dados ){
        global $db;

        $qstid = $dados['qstid'];

        if( $dados['tipo_fil'] == 'fil' ){
            if($dados['prgdsc_pes']){
                $prgdsc_pes = removeAcentos(trim($dados['prgdsc_pes']));
                $where .= " AND public.removeacento(p.prgdsc) ilike public.removeacento('%{$prgdsc_pes}%') ";
            }
        }else{
            $where = "";
        }

        $acao = "
            <img border=\"0\" title=\"Excluir Pergunta\" onclick=\"excluirPerguntaQuest('|| p.prgid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
            <img border=\"0\" title=\"Alterar Pergunta\" onclick=\"alerterarPerguntaQuest('|| p.prgid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/alterar.gif\" />
        ";

        $sql = "
            SELECT  '{$acao}',
                    prgid,
                    prgdsc,
                    prgtipo,
                    prgtamanho,

                    CASE WHEN prgobrigatoriedade = 't'
                        THEN '<span style=\"color: #00008B;\"><b> Sim </spam>'
                        ELSE '<span style=\"color: #8B3A3A;\"> Não </spam>'
                    END AS prgobrigatoriedade,

                    CASE WHEN (prgmascara = '' OR prgmascara IS NULL)
                        THEN '<span style=\"color: #8B1A1A;\"> N/A </spam>'
                        ELSE '<span style=\"color: #1E90FF;\"><b>' || prgmascara || '</b></spam>'
                    END prgmascara,

                    CASE WHEN prgiditem IS NULL
                        THEN '<span style=\"color: #EE2C2C;\"> N/A </spam>'
                        ELSE '<span style=\"color: #1C86EE;\"><b>' || prgiditem || '</b></spam>'
                    END prgiditem,

                    CASE WHEN prgmodalidade = 'N'
                        THEN '<span style=\"color: #CD2626;\"> N/A </spam>'
                        ELSE '<span style=\"color: #1874CD;\"><b>' || prgmodalidade || '</b></spam>'
                    END prgmodalidade,

                    CASE WHEN prgadicionaarq = 't'
                        THEN '<span style=\"color: #27408B;\"><b> Sim </b></spam>'
                        ELSE '<span style=\"color: #EE3B3B;\"> N/A </spam>'
                    END prgadicionaarq,

                    CASE WHEN prgstatus = 'I'
                        THEN '<span style=\"color: 8B1A1A;\"> I </spam>'
                        ELSE '<span style=\"color: 63B8FF;\"><b> A </b></spam>'
                    END prgstatus,

                    TO_CHAR(prgdtinclusao, 'dd/mm/YYYY') AS prgdtinclusao
            FROM maismedicomec.pergunta AS p
            JOIN maismedicomec.questionario AS q ON q.qstid = p.qstid

            WHERE p.qstid = {$qstid} {$where}

            ORDER BY prgid;
        ";
        $cabecalho = Array("Ação", "ID Perg", "Descrição", "Tipo", "Tamanho", "Obrigatorio", "Mascara", "Pai", "Modalidade", "Anexo", "Satatus", "Incluido" );
        $whidth = Array('3%', '3%', '', '', '', '', '', '', '', '', '', '', '');
        $align  = Array('center', 'center', 'left', '', 'center', 'center', 'center', 'center', 'center', 'center', 'center', 'center', 'center');

        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', NULL, $whidth, $align, '');
    }

    function salvarPerguntasQuest( $dados ){
        global $db;

        extract($dados);

        $prgtamanho = $prgtamanho ? "{$prgtamanho}" : "NULL";
        $prgmascara = $prgmascara ? "'{$prgmascara}'" : "NULL";
        $prgiditem  = $prgiditem  ? "'".trim($prgiditem)."'" : "NULL";

        if( $prgid == '' ){
            $sql = "
                INSERT INTO maismedicomec.pergunta(
                        qstid, prgdsc, prgtipo, prgtamanho, prgobrigatoriedade, prgmascara, prgstatus, prgiditem, prgmodalidade, prgadicionaarq
                    )VALUES (
                        {$qstid}, '{$prgdsc}', '{$prgtipo}', {$prgtamanho}, '{$prgobrigatoriedade}', {$prgmascara}, '{$prgstatus}', {$prgiditem}, '{$prgmodalidade}', '{$prgadicionaarq}'
                ) RETURNING prgid;
            ";
        }else{
            $sql = "
                UPDATE maismedicomec.pergunta
                    SET prgdsc              = '{$prgdsc}',
                        prgtipo             = '{$prgtipo}',
                        prgtamanho          = {$prgtamanho},
                        prgobrigatoriedade  = '{$prgobrigatoriedade}',
                        prgmascara          = {$prgmascara},
                        prgstatus           = '{$prgstatus}',
                        prgiditem           = {$prgiditem},
                        prgmodalidade       = '{$prgmodalidade}',
                        prgadicionaarq      = '{$prgadicionaarq}'
                  WHERE prgid = {$prgid} AND qstid = {$qstid} RETURNING prgid;
            ";
        }
        $prgid = $db->pegaUm($sql);

        if($prgid){
            $db->commit();
            $parametros = "&qstid=".$qstid;
            $db->sucesso('sistema/tabela_apoio/cad_monta_perguntas', $parametros, 'Pergunta cadastrado com sucesso!', 'N', 'N');
        } else {
            $db->insucesso('Não foi possível realizar a operação.', '', 'sistema/tabela_apoio/cad_monta_perguntas&acao=A');
        }
    }


    #----------------
    $qstid = $_REQUEST['qstid'];

    if($qstid != ''){
        $sql = "SELECT qstdsc FROM maismedicomec.questionario WHERE qstid = {$qstid}";
        $qstdsc = $db->pegaUm($sql);
    }

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo('Instrumento de Avaliação', 'Tela de Apoio para a montagem das perguntas' );

    $abacod_tela    = ABA_GERAR_QUESTIONARIO_PERGUNTA;
    $url            = 'maismedicomec.php?modulo=sistema/tabela_apoio/cad_monta_perguntas&acao=A';
    $parametros     = '';
    $db->cria_aba($abacod_tela, $url, $parametros);
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

    function abrirPesquisa(opc){
        if(opc == 'mais'){
            //ABRE AS TR
            $('#tr_descricao_pergunta').css("display", "");
            $('#tr_botao').css("display", "");
            //MUDAR A IMAGEM E O VALOR.
            $('#sinal_mais').css("display", "none");
            $('#sinal_menos').css("display", "");
        }else{
            //ESCONDE AS TR
            $('#tr_descricao_pergunta').css("display", "none");
            $('#tr_botao').css("display", "none");
            //MUDAR A IMAGEM E VALOR
            $('#sinal_mais').css("display", "");
            $('#sinal_menos').css("display", "none");
        }
    }

    function alerterarPerguntaQuest( prgid ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=alerterarPerguntaQuest&prgid="+prgid,
            success: function(resp){
                var dados = $.parseJSON(resp);

                $.each(dados, function(index, value){
                    $.each(value, function(index, value) {
                        $('#'+index).val(value);
                    });
                });
            }
        });
    }

    function excluirPerguntaQuest( prgid ){
        var confirma = confirm("Deseja realmente excluir a pergunta? Esse processo excluirá fisicamente a pergunta. Caso queira inativa-lá, altere o seu status!");
        if( confirma ){
            $('#prgid').val( prgid );
            $('#requisicao').val('excluirPerguntaQuest');
            $('#formulario').submit();
        }
    }

    function salvarPerguntasQuest( param ){
        var erro;
        var campos = '';

        $.each($(".obrigatorio"), function(i, v){
            if( $(this).attr('type') != 'radio' ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else{
                var name  = $(this).attr('name');
                var value = $(this).attr('value');
                var radio_box = $('input:radio[name='+name+']:checked');

                if(!radio_box.val()){
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
            $('#requisicao').val('salvarPerguntasQuest');
            $('#formulario').submit();
        }
    }

    function pesquisarOrgao(param){
        if(trim(param) == 'fil'){
            $('#tipo_fil').val('fil');
            $('#formulario').submit();
        }else{
            $('#tipo_fil').val('');
            $('#formulario').submit();
        }
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="qstid" name="qstid" value="<?=$qstid;?>"/>
    <input type="hidden" id="tipo_fil" name="tipo_fil" value="<?=$qstid;?>"/>

    <table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">
        <tr>
            <td colspan="2" class="subTituloCentro" style="text-align: left;">
                <img id="sinal_mais"  style="cursor: pointer; " src="../imagens/mais.gif" onclick="abrirPesquisa('mais');">
                <img id="sinal_menos" style="display: none; cursor: pointer;" src="../imagens/menos.gif" onclick="abrirPesquisa('menos');">
                &nbsp;&nbsp;
                PESQUISAR PERGUNTAS - DIGITE A PERGUNTA OU PARTE DELA.
            </td>
        </tr>
        <tr id="tr_descricao_pergunta" style="display: none;">
            <td class = "subtitulodireita" width="33%">Descrição da pergunta:</td>
            <td>
                <?PHP
                    echo campo_texto('prgdsc_pes', 'N', 'S', 'Descrição da pergunta', 100, 1000, '', '', '', '', '', 'id="prgdsc_pes"', '', $prgdsc_pes);
                ?>
            </td>
        </tr>
        <tr id="tr_botao" style="display: none;">
            <td class="SubTituloDireita" colspan="2" style="text-align:center">
                <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisarOrgao('fil');">
                <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisarOrgao('tudo');">
            </td>
        </tr>
    </table>

    <br>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
        <tr>
            <td class ="SubTituloCentro" colspan="4" style="height: 35px;"> Montagem de Perguntas para o Questionário</td>
        </t>
        <tr>
            <td class ="SubTituloDireita" width="25%"> ID do Questionario </td>
            <td width="25%">
               <?PHP
                    echo campo_texto('qstid', 'N', 'N', 'ID do Questionario', 20, 10, '', '', '', '', 0, 'id="qstid"', '', $qstid, null, '', null);
                ?>
            </td>
            <td class ="SubTituloDireita" width="10%">  Questionário </td>
            <td>
               <?PHP
                    echo campo_texto('qstdsc', 'N', 'N', 'ID da Pergunta', 20, 10, '', '', '', '', 0, 'id="qstdsc"', '', $qstdsc, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" colspan="4"> &nbsp; </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="10%"> ID da Pergunta </td>
            <td>
               <?PHP
                    echo campo_texto('prgid', 'N', 'N', 'ID da Pergunta', 20, 10, '', '', '', '', 0, 'id="prgid"', '', $prgid, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="25%"> Descrição da Pergunta </td>
            <td colspan="3">
               <?PHP
                    echo campo_textarea('prgdsc', 'S', 'S', '', 100, 3, 1000, '', 0, '', false, 'Descrição da Pergunta', $prgdsc, '67%', "prgdsc");
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="25%"> Tipo da Pergunta </td>
            <td colspan="3">
               <?PHP
                    $sql = array(
                        array(codigo => "T", descricao => "Texto Longo"),
                        array(codigo => "C", descricao => "Texto Curto"),
                        array(codigo => "D", descricao => "Tipo Data"),
                        array(codigo => "M", descricao => "Valor Monetário"),
                        array(codigo => "N", descricao => "Valor Númerico"),
                        array(codigo => "B", descricao => "Valor Booleano")
                    );
                    $db->monta_combo("prgtipo", $sql, 'S', 'Selecione...', '', '', '', '333', 'S', 'prgtipo', false, $prgtipo, 'Tipo da Pergunta');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="25%"> Tamanho </td>
            <td colspan="3">
               <?PHP
                    echo campo_texto('prgtamanho', 'N', 'S', 'Tamanho', 33, 8, '########', '', '', '', 0, 'id="prgtamanho"', '', $prgtamanho, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="25%"> Obrigatório - Sim/ Não </td>
            <td colspan="3">
               <?PHP
                    $sql = array(
                        array(codigo => "t", descricao => "Sim"),
                        array(codigo => "f", descricao => "Não")
                    );
                    $db->monta_combo("prgobrigatoriedade", $sql, 'S', 'Selecione...', '', '', '', '333', 'S', 'prgobrigatoriedade', false, $prgobrigatoriedade, 'Obrigatório - Sim/ Não');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="25%"> Mascara </td>
            <td colspan="3">
               <?PHP
                    echo campo_texto('prgmascara', 'N', 'S', 'Mascara', 33, 12, '', '', '', '', 0, 'id="prgmascara"', '', $prgmascara, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="25%"> ID Pergunta "Pai" </td>
            <td colspan="3">
               <?PHP
                    echo campo_texto('prgiditem', 'N', 'S', 'ID Pergunta "Pai"', 33, 12, '############', '', '', '', 0, 'id="prgiditem"', '', $prgiditem, null, '', null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="25%"> Modalidade </td>
            <td colspan="3">
               <?PHP
                    $sql = array(
                        array(codigo => "N", descricao => "Não se Aplica"),
                        array(codigo => "H", descricao => "Hospital"),
                        array(codigo => "U", descricao => "UBS"),
                        array(codigo => "P", descricao => "UPA"),
                        array(codigo => "C", descricao => "CAPS"),
                        array(codigo => "A", descricao => "Ambulatório")
                    );
                    $db->monta_combo("prgmodalidade", $sql, 'S', 'Selecione...', '', '', '', '333', 'S', 'prgmodalidade', false, $prgmodalidade, 'Modalidade');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="25%"> Perguta tera Anexo - Sim/ Não </td>
            <td colspan="3">
               <?PHP
                    $sql = array(
                        array(codigo => "t", descricao => "Sim"),
                        array(codigo => "f", descricao => "Não")
                    );
                    $db->monta_combo("prgadicionaarq", $sql, 'S', 'Selecione...', '', '', '', '333', 'S', 'prgadicionaarq', false, $prgadicionaarq, 'Perguta tera Anexo - Sim/ Não');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="25%"> Status da Pergunta </td>
            <td colspan="3">
               <?PHP
                    $sql = array(
                        array(codigo => "A", descricao => "Ativo"),
                        array(codigo => "I", descricao => "Inativo")
                    );
                    $db->monta_combo("prgstatus", $sql, 'S', '', '', '', '', '333', 'S', 'prgstatus', false, $prgadicionaarq, 'Status da Pergunta');
                ?>
            </td>
        </tr>
    </table>

    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1" class="Tabela listagem">
        <tr>
            <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
                <!-- <input type="button" id="cancela" name="cancela" value="Cancelar" onclick="voltarPerguntas();"/> -->
                <input type="button" id="salvarPerguntas" name="salvarPerguntas" value="Salvar" onclick="salvarPerguntasQuest('S');"/>
            </td>
        </tr>
    </table>
</form>

<div id="div_listagem_perguntas_questionario">
    <?PHP
        //listagemPerguntaQuest( $qstid );
        listagemPerguntaQuest( $_REQUEST );
    ?>
</div>