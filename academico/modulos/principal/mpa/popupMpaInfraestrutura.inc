<?php

    unset($_SESSION['arrayObras']);

    if ($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        die();
    }

    $dinid = $_REQUEST['dinid'];

    if($dinid != ''){
        $sql = "
            SELECT  d.dinid,
                    entidunidade,
                    entidcampus,
                    dindsc,
                    dintipo,
                    dindtinclusao,
                    dinstatus,
                    dindtprevisaotermino,
                    speid,
                    trim(to_char(dinvlr2014, '999G999G999G999G999G999G990D99')) as dinvlr2014,
                    trim(to_char(dinvlr2015, '999G999G999G999G999G999G990D99')) as dinvlr2015,
                    trim(to_char(dinvlr2016, '999G999G999G999G999G999G990D99')) as dinvlr2016,
                    trim(to_char(dinvlr2017, '999G999G999G999G999G999G990D99')) as dinvlr2017,
                    dinsitlicitacao,
                    dinasldscoutros,
                    dindtprevisaolicitacao,
                    -- Aguardando dotação orçamentária
                    a1.talid AS talid_1,
                    a1.daldsc AS daldscorcamentaria,
                    --Revisão do Planejamento Institucional
                    a2.talid AS talid_2,
                    a2.daldsc AS  daldscinstitucional,
                    --Outros
                    a3.talid AS talid_3,
                    a3.daldsc  AS daldscoutros
            FROM academico.demandainfra AS d

            LEFT JOIN academico.acaolicitacao AS a1 ON a1.dinid = d.dinid AND a1.talid = 1
            LEFT JOIN academico.acaolicitacao AS a2 ON a2.dinid = d.dinid AND a2.talid = 2
            LEFT JOIN academico.acaolicitacao AS a3 ON a3.dinid = d.dinid AND a3.talid = 3

            WHERE d.dinid = {$dinid}
        ";
        $dados = $db->pegaLinha($sql);

        $sql = "
            SELECT  d.tpdid as codigo,
                    t.tpddsc as descricao
            FROM academico.demandatipologia d
            JOIN academico.tipologiademanda t on t.tpdid = d.tpdid
            WHERE dinid = {$dinid};
        ";
        $dados_tipo = $db->carregar($sql);
    }

    #ATUALIZA O COMBO POP DE ACORDO COM  A OP?ÃO. '0'-OBRAS E 'I'-OUTRAS
    function atualizaComboTipologia( $dados ){
        global $db, $tpdid;

        $dintipo = $dados['dintipo'];
        $dinid   = $dados['dinid'];

        if($dinid != ''){
            $sql = "
                SELECT  dintipo
                FROM academico.demandainfra
                WHERE dinid = {$dinid}
            ";
            $bd_dintipo = $db->pegaUm($sql);
        }

        if( $dinid != '' && $dintipo == $bd_dintipo ){
            $sql = "
                SELECT  d.tpdid as codigo,
                        t.tpddsc as descricao
                FROM academico.demandatipologia d
                JOIN academico.tipologiademanda t on t.tpdid = d.tpdid
                WHERE dinid = {$dinid};
            ";
            $tpdid = $db->carregar($sql);
        }else{
            $tpdid = '';
        }

        switch ( $dintipo ){
            case 'I':
                $where = " WHERE tpdtipo = 'I' ";
                break;
            case 'O':
                $where = " WHERE tpdtipo = 'O' ";
                break;
            default:
                $where = " WHERE tpdtipo IN ( 'I', 'O' )";
        }

        $sql_tpdid = "
            SELECT  tpdid as codigo,
                    tpddsc as descricao
            FROM academico.tipologiademanda
            {$where}
        ";
        combo_popup( 'tpdid', $sql_tpdid, 'Selecione a(s) Tipologias(s)', '400x400', 0, '', '', 'S', false, false, 10, 400, null, null, false, null, $tpdid);
        die();
    }

    #EXCLUI AS OBRAS RELACIONADAS COM A DEMANDA.
    function excluirDemandasObras( $dados ){
        global $db;

        $obrid = $dados['obrid'];
        $dinid = $dados['dinid'];

        $sql = "
            DELETE FROM academico.demandaobras WHERE obrid = {$obrid} AND dinid = {$dinid} RETURNING obrid;
        ";
        $obrid = $db->pegaUm($sql);

        if($obrid > 0){
            $db->commit();
            $retorno["resultado"] = "S";
        } else {
            $db->rollback();
            $retorno["resultado"] = "N";
        }
	echo simec_json_encode($retorno);
        die();
    }

    #FORMATA A DATA PADRÃO BR PARA O PADRÃO AMERICANO (PREPARA PARA O BANCO DE DADOS).
    function formataDataBanco($valor){
        $data = explode("/",$valor);
        $dia = $data[0];
        $mes = $data[1];
        $ano = $data[2];
        return $ano."-".$mes."-".$dia;
    }

    #SALVA DAODOS DA DEMANDA DE INFRA - NAS RESPECTIVAS TABELAS - academico.demandainfra (DADOS DA DEMANDA) E academico.demandatipologia (DADOS DA TIPOLOGIA)
    function salvarDadosDemanda() {
        global $db;

        //ver($_POST, d);

        #DADOS DEMANDA INFRA.
        $entid                  = $_SESSION['academico']['entid'];

        $dinid                  = $_POST['dinid'];
        $entidcampus            = $_POST['entidcampus'];
        $dintipo                = $_POST['dintipo'];
        $dindsc                 = $_POST['dindsc'];
        $speid                  = $_POST['speid'] == '' ? 'NULL' : $_POST['speid'];
        $dindtprevisaotermino   = $_POST['dindtprevisaotermino'] == '' ? 'NULL' : "'".formataDataBanco( $_POST['dindtprevisaotermino'] )."'";
        $dinvlr2014             = $_POST['dinvlr2014'] == '' ? '0.00' : formata_valor_sql( $_POST['dinvlr2014'] );
        $dinvlr2015             = $_POST['dinvlr2015'] == '' ? '0.00' : formata_valor_sql( $_POST['dinvlr2015'] );
        $dinvlr2016             = $_POST['dinvlr2016'] == '' ? '0.00' : formata_valor_sql( $_POST['dinvlr2016'] );
        $dinvlr2017             = $_POST['dinvlr2017'] == '' ? '0.00' : formata_valor_sql( $_POST['dinvlr2017'] );

        $dinsitlicitacao        = $_POST['dinsitlicitacao'] == '' ? '' : $_POST['dinsitlicitacao'];
        $dinasldscoutros        = $_POST['dinasldscoutros'] == '' ? '' : $_POST['dinasldscoutros'];
        $dindtprevisaolicitacao = $_POST['dindtprevisaolicitacao'] == '' ? 'NULL' : "'".formataDataBanco( $_POST['dindtprevisaolicitacao'] )."'";

        $daldscorcamentaria     = $_POST['daldscorcamentaria'];
        $daldscinstitucional    = $_POST['daldscinstitucional'];
        $daldscoutros           = $_POST['daldscoutros'];

        #DADOS DEMANDA TIPOLOGIA
        $tpdid = $_POST['tpdid'];

        #AÇÃO SITUAÇÃO LICITAÇÃO.
        $talid = $_POST['talid'];

        #DADOS OBRAS SELECIONADAS
        $obrid = $_POST['obrid'];

        #USUÁRIO - DA SESSÃO.
        $usucpf = $_SESSION['usucpf'];

        if ($dinid != '') {
            $sql = "
                UPDATE academico.demandainfra SET
                        entidcampus             = $entidcampus,
                        dindsc                  = '{$dindsc}',
                        dintipo                 = '{$dintipo}',
                        speid                   = $speid,
                        dindtprevisaotermino    = {$dindtprevisaotermino},
                        dinvlr2015              = '{$dinvlr2015}',
                        dinvlr2016              = '{$dinvlr2016}',
                        dinvlr2017              = '{$dinvlr2017}',
                        dinvlr2014              = '{$dinvlr2014}',
                        dinsitlicitacao         = '{$dinsitlicitacao}',
                        dinasldscoutros         = '{$dinasldscoutros}',
                        dindtprevisaolicitacao  = {$dindtprevisaolicitacao},
                        usucpf                  = '{$usucpf}'
                WHERE dinid = {$dinid} RETURNING dinid;
            ";
        } else {
            $sql = "
                INSERT INTO academico.demandainfra(
                        entidunidade,
                        entidcampus,
                        dindsc,
                        dintipo,
                        speid,
                        dindtprevisaotermino,
                        dinvlr2015,
                        dinvlr2016,
                        dinvlr2017,
                        dinvlr2014,
                        dinsitlicitacao,
                        dinasldscoutros,
                        dindtprevisaolicitacao,
                        usucpf
                    ) VALUES (
                        {$entid},
                        {$entidcampus},
                        '{$dindsc}',
                        '{$dintipo}',
                        {$speid},
                        {$dindtprevisaotermino},
                        '{$dinvlr2015}',
                        '{$dinvlr2016}',
                        '{$dinvlr2017}',
                        '{$dinvlr2014}',
                        '{$dinsitlicitacao}',
                        '{$dinasldscoutros}',
                        {$dindtprevisaolicitacao},
                        '{$usucpf}'
                    )RETURNING dinid;
            ";
        }
        $dados_dinid = $db->pegaUm($sql);

        #TIPOLOGIAS
        if( $dados_dinid > 0 ){
            if($dinid != ''){
                $sql = "
                    DELETE FROM academico.demandatipologia WHERE dinid = {$dinid};
                ";
                $db->executar($sql);
            }

            $a = 0;
            foreach( $tpdid as $k){
                if( $tpdid[$a] != ''){
                    $sql_tipol .= "
                        INSERT INTO academico.demandatipologia( tpdid, dinid ) VALUES ( {$k}, {$dados_dinid} ) RETURNING dtpid;
                    ";
                }
                $a = $a + 1;
            }
            $dado = $db->pegaUm($sql_tipol);
        }

        #AÇÃO SITUAÇÃO LICITAÇÃO
        if( $dados_dinid > 0 ){
            if($dinid != ''){
                $sql = "
                    DELETE FROM academico.acaolicitacao WHERE dinid = {$dinid};
                ";
                $db->executar($sql);
            }

            if( $talid != '' ){
                $a = 0;
                foreach( $talid as $k){
                    switch ($talid[$a]){
                        case 1:
                            $daldsc = $daldscorcamentaria;
                            break;
                        case 2:
                            $daldsc = $daldscinstitucional;
                            break;
                        case 3:
                            $daldsc = $daldscoutros;
                            break;
                        default:
                            $daldsc = "";
                    }

                    if( $talid[$a] != ''){
                        $sql_acao .= "
                            INSERT INTO academico.acaolicitacao( dinid, talid, daldsc ) VALUES ( {$dados_dinid}, {$k}, '{$daldsc}' ) RETURNING talid;
                        ";
                    }
                    $a = $a + 1;
                }
                $dado = $db->pegaUm($sql_acao);
            }
        }

        #OBRAS
        if( $dados_dinid > 0 ){
            if($dinid != ''){
                $sql = "
                    DELETE FROM academico.demandaobras WHERE dinid = {$dinid};
                ";
                $db->executar($sql);
            }

            if( $obrid != '' ){
                $a = 0;
                foreach( $obrid as $k){
                    if( $obrid[$a] != ''){
                        $sql_obras .= "
                            INSERT INTO academico.demandaobras (obrid, dinid) VALUES ( {$k}, {$dados_dinid} ) RETURNING obrid;
                        ";
                    }
                    $a = $a + 1;
                }
                $dado = $db->pegaUm($sql_obras);
            }
        }

        if( $dado > 0 ){
            $db->commit();

            unset($_SESSION['arrayObras']);

            $db->sucesso( 'principal/mpa/popupMpaInfraestrutura', '&dinid='.$dados_dinid.'&entidcampus='.$entidcampus );
        }else{
            $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/mpa/popupMpaInfraestrutura&acao=A');
        }
    }

    function montaListaObras( $entidcampus, $dinid ){
        global $db;
        $acao = "
            '<img border=\"0\" title=\"Apagar Obra\" onclick=\"excluirDemandasObras('|| oi.obrid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
             <input type=\"hidden\" name=\"obrid[]\" value=\"'|| oi.obrid ||'\">'
        ";

        $sql = "
                SELECT  {$acao} AS acao,
                        '<a href=/obras/obras.php?modulo=principal/cadastro&acao=A&obrid='|| oi.obrid || '>' || ee.entnome || '</a>' as nome_universidade,
                        ec.entnome as nome_campus,
                        oi.obrid,
                        upper(oi.obrdesc) as Nome_Da_Obra,
                        st.stodesc as SituacaoObra,
                        oi.obrpercexec as Percentual_Executado

                FROM obras.obrainfraestrutura AS oi

                JOIN entidade.entidade AS ee ON oi.entidunidade = ee.entid
                JOIN entidade.entidade AS ec ON oi.entidcampus = ec.entid
                JOIN entidade.endereco AS ed ON oi.endid = ed.endid
                JOIN territorios.municipio AS tm ON ed.muncod = tm.muncod
                JOIN obras.situacaoobra  AS st ON oi.stoid = st.stoid

                JOIN academico.demandaobras AS dd ON dd.obrid = oi.obrid

                LEFT JOIN(
                    SELECT  rsuid,
                            obrid,
                            supdtinclusao
                    FROM obras.supervisao s
                    WHERE supvid = (SELECT max(supvid) FROM obras.supervisao ss WHERE ss.obrid = s.obrid)
                ) AS su ON su.obrid = oi.obrid

                WHERE oi.obsstatus = 'A' AND oi.orgid = 1 AND oi.entidcampus = {$entidcampus} AND dinid = {$dinid}

                ORDER BY ee.entnome
        ";
        $cabecalho = Array("&nbsp", "Universidade",  "Campus","ID Obra","Nome da Obra", "Situação", "% Executado");
        $whidth = Array('3%', '', '','', '', '');
        $align  = Array('center', '', '', '', '', '');

        if($entidcampus != '' && $dinid != ''){
            $db->monta_lista($sql, $cabecalho, 15, 10, 'N', '', 'N', '', $whidth, $align, '');
        }
    }

    monta_titulo('Demanda', 'Cadastro de Tipologias por Demanda');
    echo '<br>';
?>

<title>Demandas/Infraestrutura</title>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" ></link>

<script language="JavaScript" type="text/javascript" src="../../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

<script>

    $(window).unload(
        function() {
            //this.opener.location.reload();
        }
    );

    function anexarArquivos(){
        var arquivo         = $('#arquivo');
        var arqdescricao    = $('#arqdescricao');
        var tpaid_arquivo   = $('#tpaid_arquivo');

        var erro;

        if(!arquivo.val()){
            alert('É necessario selecionar um arquivo!');
            arquivo.focus();
            erro = 1;
            return false;
        }
        if(!arqdescricao.val()){
            alert('O campo "Descrição do arquivo" é um campo obrigatório!');
            arqdescricao.focus();
            erro = 1;
            return false;
        }
        if(!tpaid_arquivo.val()){
            alert('O campo "Tipo do arquivo" é um campo obrigatório!');
            tpaid_arquivo.focus();
            erro = 1;
            return false;
        }

        if(!erro){
            $('#requisicao').val('salvarArquivoInfra');
            $('#formulario').submit();
        }
    }

    function abrirPopSelecaoObras(){
        var entidcampus = $('#entidcampus').val();
        var dinid = $('#dinid').val();
        if( entidcampus > 0 ){
            var janela = window.open( 'academico.php?modulo=principal/mpa/cad_demanda_infra_selecao_obra&acao=A&entidcampus='+entidcampus+'&dinid='+dinid, 'seleciona_obras', 'width=800,height=800,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
        }else{
            alert('Por favor é necessário selecionar um "CAMPUS"!');
        }
        janela.focus();
    }

    function atualizaComboTipologia( dintipo ){
        var dinid = $('#dinid').val();

        if(dintipo == 'I'){
            $('#texto_situacao').html('Situação do Termo de referência:');
            $('#dindtprevisaotermino').attr('disabled', true);
            $('#previsaoTermino').css("display", "none");
        }else{
            $('#texto_situacao').html('Situação do Projeto Executivo:');
            $('#dindtprevisaotermino').attr('disabled', false);
            $('#previsaoTermino').css("display", "");
        }

        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboTipologia&dintipo="+dintipo+"&dinid="+dinid,
            async: false,
            success: function(resp){
                $('#comboSelectTipografia').html(resp);
            }
        });
    }

    function excluirDemandasObras( obrid ){
        var dinid = $('#dinid').val();

        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
            jQuery.ajax({
                url : 'academico.php?modulo=principal/mpa/popupMpaInfraestrutura&acao=A',
                dataType: 'json',
                data: {
                    requisicao  : 'excluirDemandasObras',
                    obrid       : obrid,
                    dinid       : dinid
                },
                async: false,
                success: function( data ){
                    if(data.resultado == 'S'){
                        alert('Exclução realizada com sucesso!');
                        $('#formulario').submit();
                    }
                }
            });
        }
    }

    function exibirOpcoesLicitacao( valor ){
        if( valor == 1 ){
            $('#tr_dinsitlicitacao').css('display', '');
            $('#tr_dindtprevisaolicitacao').css('display', '');
        }else{
            $('#tr_dinsitlicitacao').css('display', 'none');
            $('#tr_talid').css('display', 'none');
            $('#tr_dinasldscoutros').css('display', 'none');
            $('#tr_dindtprevisaolicitacao').css('display', 'none');

            $('#tr_botao_selecionar_obras').css('display', 'none');
            $('#tr_listagem_obras_selecionadas').css('display', 'none');
        }
    }

    function exibirAcaoSituacaoLicitacao( valor ){
        if( valor == 1 ){
            $('#tr_talid').css('display', '');
            $('#tr_dinasldscoutros').css('display', '');
            $('#tr_dindtprevisaolicitacao').css('display', '');

            $('#tr_botao_selecionar_obras').css('display', '');
            $('#tr_listagem_obras_selecionadas').css('display', '');
        }else{
            $('#tr_talid').css('display', 'none');
            $('#tr_dinasldscoutros').css('display', 'none');
            //$('#tr_dindtprevisaolicitacao').css('display', 'none');

            $('#tr_botao_selecionar_obras').css('display', '');
            $('#tr_listagem_obras_selecionadas').css('display', '');
        }
        //tr_botao_selecionar_obras
    }

    function exibirDescricaoAcao( valor ){
        var check = $('#talid_'+valor);

        if( valor == 1 && check.attr('checked') == true ){
            $('#tr_daldscorcamentaria').css('display', '');
        }else
            if( valor == 1 && check.attr('checked') == false ){
            $('#tr_daldscorcamentaria').css('display', 'none');
        }

        if( valor == 2 && check.attr('checked') == true ){
            $('#tr_daldscinstitucional').css('display', '');
        }else
            if( valor == 2 && check.attr('checked') == false ){
            $('#tr_daldscinstitucional').css('display', 'none');
        }

        if( valor == 3 && check.attr('checked') == true ){
            $('#tr_daldscoutros').css('display', '');
        }else
             if( valor == 3 && check.attr('checked') == false){
            $('#tr_daldscoutros').css('display', 'none');
        }
    }

    function excluirArquivoInfra(arqid){
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
            $('#requisicao').val('excluirArquivoInfra');
            $('#arqid').val( arqid );
            $('#formulario').submit();
        }
    }

    function downloadArquivoInfra( arqid ){
        $('#requisicao').val('downloadArquivoInfra');
        $('#arqid').val( arqid );
        $('#formulario').submit();
    }

    function salvarDadosDemanda(){
        var entidcampus             = $('#entidcampus');
        var dinid                   = $('#dinid');
        var tpdid                   = $('#tpdid option:eq(0)');
        var dintipo                 = $('input:radio[name=dintipo]:checked');
        var dindsc                  = $('#dindsc');
        var dinsitlicitacao         = $('input:radio[name=dinsitlicitacao]:checked');
        var talid                   = $('input:checkbox[name=talid[]]:checked');
        var dindtprevisaolicitacao  = $('#dindtprevisaolicitacao');
        var formulario_obras        = $('#formulario_obras');
        var obrid                   = $('input:hidden[name=obrid[]]');
        var dindtprevisaotermino    = $('#dindtprevisaotermino');
        var dinvlr2014              = $('#dinvlr2014');
        var dinvlr2015              = $('#dinvlr2015');
        var dinvlr2016              = $('#dinvlr2016');
        var dinvlr2017              = $('#dinvlr2017');

        var formulario = document.formulario;

        var erro;

        if( !dinid.val() ){
            if(!entidcampus.val()){
                alert('O campo "Campus" é um campo obrigatório!');
                entidcampus.focus();
                erro = 1;
                return false;
            }
        }
        if(!dintipo.val()){
            alert('O campo "Tipo" é um campo obrigatório!');
            dintipo.focus();
            erro = 1;
            return false;
        }
        if(!dindsc.val()){
            alert('O campo "Dados da Demanda" é um campo obrigatório!');
            dindsc.focus();
            erro = 1;
            return false;
        }
	if (!tpdid.val()){
            alert('O campo "Tipologia" é um campo obrigatório!');
            tpdid.focus();
            erro = 1;
            return false;
	}
        /*
        if(!dinsitlicitacao.val()){
            alert('O campo "Situação da Licitação" é um campo obrigatório!');
            dinsitlicitacao.focus();
            erro = 1;
            return false;
        }

        if(dinsitlicitacao.val() == 'L'){
            if(!obrid.val()){
                alert('O campo "Lista de Obras" é um campo obrigatório!');
                obrid.focus();
                erro = 1;
                return false;
            }
        }

        if(dinsitlicitacao.val() == 'N'){
             if(!talid.val()){
                alert('O campo "Ação Situação Licitação" é um campo obrigatório!');
                talid.focus();
                erro = 1;
                return false;
            }
            if(!dindtprevisaolicitacao.val()){
               alert('O campo "Previsão do Término da Licitação da Obra Demandada" é um campo obrigatório!');
               dindtprevisaolicitacao.focus();
               erro = 1;
               return false;
           }
           if(!obrid.val()){
                alert('O campo "Lista de Obras" é um campo obrigatório!');
                obrid.focus();
                erro = 1;
                return false;
            }
        }

        if( !dindtprevisaotermino.val() && dintipo.val() == 'O' ){
            alert('O campo "Previsão do Término do Projeto Executivo" é um campo obrigatório!');
            dindtprevisaotermino.focus();
            erro = 1;
            return false;
        }

        if( dinvlr2014.val() || dinvlr2015.val() || dinvlr2016.val() || dinvlr2017.val() ){
            erro = 0;
        }else{
            alert('O campo "Valores" é um campo obrigatório, pelo menos o preenchimento de um!');
            dinvlr2014.focus();
            erro = 1;
            return false;
        }
        */
        if(!erro){
            selectAllOptions( formulario.tpdid );

            $('#requisicao').val('salvarDadosDemanda');
            $('#formulario').submit();
        }
    }

</script>

<form name="formulario" id="formulario" method="POST" enctype="multipart/form-data">
    <input type="hidden" value="" name="requisicao" id="requisicao" />
    <input type="hidden" value="<?= $dinid; ?>" name="dinid" id="dinid" />
    <input type="hidden" value="" name="arqid" id="arqid" />

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        <tr>
            <td valign="top" class="SubtituloDireita" width="25%" >Campus:</td>
            <td>
                <?PHP
                    switch ($_SESSION['academico']['orgid']) {
                        case '1':
                            $funid = ACA_ID_CAMPUS;
                            break;
                        case '2':
                            $funid = ACA_ID_UNED;
                            break;
                    }

                    if ($funid) {
                        $boq = 'S';
                        if ( $dinid ) {
                            $sql = "
                                SELECT  e.entid as codigo,
                                        e.entnome as descricao
                                FROM entidade.entidade e2
                                INNER JOIN entidade.entidade e ON e2.entid = e.entid
                                INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
                                INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
                                LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
                                LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
                                WHERE ea.entid = {$_SESSION['academico']['entid']} AND e.entstatus = 'A' AND ef.funid = {$funid} AND e.entid = {$dados['entidcampus']}
                                ORDER BY e.entnome
                            ";
                            $nome = $db->pegaLinha( $sql );
                        }else{
                            $sql = "
                                SELECT e.entid as codigo,
                                       e.entnome as descricao
                                FROM entidade.entidade e2
                                INNER JOIN entidade.entidade e ON e2.entid = e.entid
                                INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
                                INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
                                LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
                                LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
                                WHERE ea.entid = {$_SESSION['academico']['entid']} AND e.entstatus = 'A' AND ef.funid = {$funid}
                                ORDER BY e.entnome
                           ";
                        }
                    }else{
                        $sql = array();
                    }
                    if ( $dinid ) {
                        echo $nome['descricao'] . '<input type="hidden" id="entidcampus" name="entidcampus" value="' . $nome['codigo'] . '"/>';
                    } else {
                        $db->monta_combo('entidcampus', $sql, $boq, 'Selecione', $acao, $opc, '', '200', 'S', 'entidcampus', $return = false, $entidcampus, $title = null);
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Tipo:</td>
            <td>
                <input type="radio" name="dintipo" id="dintipo" class="radio" value="O" onclick="atualizaComboTipologia(this.value);" <?= ($dados['dintipo'] == 'O' ? 'checked="checked"' : '') ?> /> OBRA
                <input type="radio" name="dintipo" id="dintipo" class="radio" value="I" onclick="atualizaComboTipologia(this.value);" <?= ($dados['dintipo'] == 'I' ? 'checked="checked"' : '') ?>/> OUTROS
            </td>
        </tr>
        <td class="SubtituloDireita">Dados da Demanda:</td>
        <td>
            <?PHP
                $dindsc = $dados['dindsc'];
                echo campo_textarea('dindsc', 'S', 'S', '', 80, 4, 400, '', 0, '', false, NULL, $dindsc, '44%')
            ?>
        </td>
    </table>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        <tr>
            <td class="SubtituloDireita" width="25%">Tipologia:</td>
            <td colspan="4" id="comboSelectTipografia">
                <?PHP
                    if($dados['dintipo'] == 'O'){
                       $where = " WHERE tpdtipo = 'O' ";
                       $texto = "Situação do Projeto Executivo:";
                    }elseif($dados['dintipo'] == 'I'){
                       $where = " WHERE tpdtipo = 'I' ";
                       $texto = "Situação do Termo de referência:";
                    }else{
                       $where = '';
                       $texto = "Situação do Projeto Executivo:";
                    }

                    $tpdid = $dados_tipo;
                    $sql_tpdid = "
                            SELECT  tpdid as codigo,
                                    tpddsc as descricao
                            FROM academico.tipologiademanda
                            {$where}
                    ";
                    combo_popup( 'tpdid', $sql_tpdid, 'Selecione a(s) Tipologias(s)', '500x400', 0, '', '', 'S', false, false, 10, 500, null, null, false, null, $tpdid);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" id="texto_situacao"> <?=$texto;?></td>
            <td colspan="4">
                <?PHP
                    $sql = "
                        SELECT  speid as codigo,
                                spedsc as descricao
                        FROM academico.sitprojetoexec
                    ";
                    $speid = $dados['speid'];
                    $db->monta_combo('speid', $sql, 'S', 'Selecione...', 'exibirOpcoesLicitacao', '', '', '200', 'N', 'speid', false, $speid, null);
                ?>
            </td>
        </tr>

        <?PHP
            if( $dados['dinid'] == '' ){
                $style = "";
            }elseif( $dados['dinid'] != '' && $dados['dintipo'] == 'O'){
                $style = "";
            }else{
                $style = "style=\"display: none;\"";
            }
        ?>
        <tr id="previsaoTermino" <?=$style;?>>
            <td class="SubtituloDireita">Previsão do Término do Projeto Executivo:</td>
            <td colspan="4">
                <?PHP
                    $dindtprevisaotermino = formata_data( $dados['dindtprevisaotermino'] );
                    echo campo_data2('dindtprevisaotermino', 'S', 'S', '', '##/##/####', '', '', $dindtprevisaotermino, '', '', 'dindtprevisaotermino');
                ?>
            </td>
        </tr>

        <!-- AS OPCÕES "SITUAÇÃO DO PE" SERÃO MOSTRADAS, CASO A SITUAÇÃO DO PROJETO EXECUTIVO SEJA CONCLUIDO. -->
        <?PHP
            if( $dados['speid'] == 1 ){
                $style = "";
            }else{
                $style = "style=\"display: none;\"";
            }

            switch( $dados['dinsitlicitacao'] ){
                case 'L':
                    $ckecked_L = 'checked="checked"';
                    break;
                case 'N':
                    $ckecked_N = 'checked="checked"';
                    break;
            }
        ?>
        <tr id="tr_dinsitlicitacao" <?=$style;?>>
            <td class="SubtituloDireita"> Situação da Licitação: </td>
            <td colspan="4">
                <input type="radio" name="dinsitlicitacao" value="L" title="Situação do PE - Licitado" <?=$ckecked_L;?> onclick="exibirAcaoSituacaoLicitacao(2);"> LICITADO
                <input type="radio" name="dinsitlicitacao" value="N" title="Situação do PE - Não Licitado" <?=$ckecked_N;?> onclick="exibirAcaoSituacaoLicitacao(1);"> NÃO LICITADO
            </td>
        </tr>
        <!-- FIM - AS OPCÕES "SITUAÇÃO DO PE" SERÃO MOSTRADAS, CASO A SITUAÇÃO DO PROJETO EXECUTIVO SEJA CONCLUIDO. -->

        <!-- AS OPCÕES "AÇÃO SITUAÇÃO LICITAÇÃO" SERÃO MOSTRADAS, CASO A SITUAÇÃO DO PE, SEJA "NÃO LICITADO OU EM LICITAÇÃO". -->
        <?PHP
            //if( $dados['dinsitlicitacao'] == 'L' || trim($dados['dinsitlicitacao']) == ''){
            if( $dados['speid'] == 1 ){
                $style = "";
            }else{
                $style = "style=\"display: none;\"";
            }
        ?>
        <tr id="tr_dindtprevisaolicitacao" <?=$style;?>>
            <td class="SubtituloDireita">Previsão do Término da Licitação da Obra Demandada:</td>
            <td colspan="4">
                <?PHP
                    $dindtprevisaolicitacao = formata_data( $dados['dindtprevisaolicitacao'] );
                    echo campo_data2('dindtprevisaolicitacao', 'S', 'S', '', '##/##/####', '', '', $dindtprevisaolicitacao, '', '', 'dindtprevisaolicitacao');
                ?>
            </td>
        </tr>
        <!-- FIM - AS OPCÕES "AÇÃO SITUAÇÃO LICITAÇÃO" SERÃO MOSTRADAS, CASO A SITUAÇÃO DO PE, SEJA "NÃO LICITADO OU EM LICITAÇÃO". -->

        <!-- AS OPCÕES "AÇÃO SITUAÇÃO LICITAÇÃO" SERÃO MOSTRADAS, CASO A SITUAÇÃO DO PE, SEJA "NÃO LICITADO OU EM LICITAÇÃO". -->
        <?PHP
            if( $dados['dinsitlicitacao'] == 'L' || trim($dados['dinsitlicitacao']) == ''){
                $style = "style=\"display: none;\"";
            }else{
                $style = "";
            }
        ?>
        <tr id="tr_talid" <?=$style;?>>
            <td class="SubtituloDireita"> Ação Situação Licitação: </td>
            <td colspan="4">
                 <?PHP
                    $sql = "
                        SELECT  talid as codigo,
                                taldsc as descricao
                        FROM academico.tipoacaolicitacao
                    ";
                    #ARRARY COM AS RESPOSTAS DO CHECKBOX.
                    $arrayMarcados = array($dados['talid_1'], $dados['talid_2'], $dados['talid_3']);
                    $db->monta_checkbox('talid[]', $sql, $arrayMarcados, '', false, 'exibirDescricaoAcao(this.value)' );
                ?>
            </td>
        </tr>
        <!-- FIM - AS OPCÕES "AÇÃO SITUAÇÃO LICITAÇÃO" SERÃO MOSTRADAS, CASO A SITUAÇÃO DO PE, SEJA "NÃO LICITADO OU EM LICITAÇÃO". -->

        <!-- CAMPO DESCRITIVO DA OPÇÃO AGUARDADNDO DOTAÇÃO ORÇAMENTÁRIO, CASO ESSA OPÇÃO SEJA SELECIONADA SE TORNA VISIVEL -->
        <?PHP
            if( $dados['talid_1'] == '' ){
                $style = "style=\"display: none;\"";
            }else{
                $style = "";
            }
        ?>
        <tr id="tr_daldscorcamentaria" <?=$style;?>>
            <td class="SubtituloDireita"> Descrição - Aguardando dotação orçamentária: </td>
            <td colspan="4">
                 <?PHP
                    $daldscorcamentaria = $dados['daldscorcamentaria'];
                    echo campo_textarea('daldscorcamentaria', 'S', 'S', '', 80, 4, 255, '', 0, '', false, NULL, $daldscorcamentaria, '44%')
                ?>
            </td>
        </tr>
        <!-- FIM - CAMPO DESCRITIVO DA OPÇÃO OUTROS, CASO ESSA OPÇÃO SEJA SELECIONADA SE TORNA VISIVEL -->

        <!-- CAMPO DESCRITIVO DA OPÇÃO OUTROS, CASO ESSA OPÇÃO SEJA SELECIONADA SE TORNA VISIVEL -->
        <?PHP
            if( $dados['talid_2'] == '' ){
                $style = "style=\"display: none;\"";
            }else{
                $style = "";
            }
        ?>
        <tr id="tr_daldscinstitucional" <?=$style;?>>
            <td class="SubtituloDireita"> Descrição - Revisão do Planejamento Institucional: </td>
            <td colspan="4">
                 <?PHP
                    $daldscinstitucional = $dados['daldscinstitucional'];
                    echo campo_textarea('daldscinstitucional', 'S', 'S', '', 80, 4, 255, '', 0, '', false, NULL, $daldscinstitucional, '44%')
                ?>
            </td>
        </tr>
        <!-- FIM - CAMPO DESCRITIVO DA OPÇÃO OUTROS, CASO ESSA OPÇÃO SEJA SELECIONADA SE TORNA VISIVEL -->

        <!-- CAMPO DESCRITIVO DA OPÇÃO OUTROS, CASO ESSA OPÇÃO SEJA SELECIONADA SE TORNA VISIVEL -->
        <?PHP
            if( $dados['talid_3'] == '' ){
                $style = "style=\"display: none;\"";
            }else{
                $style = "";
            }
        ?>
        <tr id="tr_daldscoutros" <?=$style;?>>
            <td class="SubtituloDireita"> Descrição - Outros: </td>
            <td colspan="4">
                 <?PHP
                    $daldscoutros = $dados['daldscoutros'];
                    echo campo_textarea('daldscoutros', 'S', 'S', '', 80, 4, 255, '', 0, '', false, NULL, $daldscoutros, '44%')
                ?>
            </td>
        </tr>
        <!-- FIM - CAMPO DESCRITIVO DA OPÇÃO OUTROS, CASO ESSA OPÇÃO SEJA SELECIONADA SE TORNA VISIVEL -->

        <!-- ADCIONAR OBRAS SELECIONADAS E LISTAGEM DAS MESMAS "OBRAS" -->
        <?PHP
            if( $dados['dinsitlicitacao'] == 'L' || $dados['dinsitlicitacao'] == 'N' || $dados['dinsitlicitacao'] == 'E'){
                $style = "";
            }else{
                $style = "style=\"display: none;\"";
            }
        ?>
        <tr id="tr_botao_selecionar_obras" <?=$style?>>
            <td class="SubTituloDireita" rowspan="2">Lista de Obras:</td>
            <td style="padding: 5px;" colspan="4">
                <span style="cursor: pointer" onclick="abrirPopSelecaoObras();" title="Selecionar Obras" >
                    <img align="absmiddle" src="/imagens/gif_inclui.gif" style="width:11px; margin-left: 5px;"/> &nbsp; Selecionar Obras
                </span>
            </td>
        </tr>
        <tr id="tr_listagem_obras_selecionadas" <?=$style?>>
            <td colspan="4" id="td_listagem_obras_relacionados">
                <?PHP
                    $entidcampus = $nome['codigo'];
                    montaListaObras( $entidcampus, $dinid );
                ?>
            </td>
        </tr>
        <!-- FIM - ADCIONAR OBRAS SELECIONADAS E LISTAGEM DAS MESMAS "OBRAS" -->

        <tr>
            <td class="SubtituloDireita">Valores:</td>
            <td width="25%"> <b>2014</b> <br>
                <?PHP
                    $dinvlr2014 = $dados['dinvlr2014'];
                    echo campo_texto('dinvlr2014', 'N', "S", 'Modelo', 30, 26, '###.###.###.###.###.###,##', '', '', '', '', 'id="dinvlr2014"', '', $dinvlr2014);
                ?>
                
                <br><br> <b>2015</b> <br>
                
                <?PHP
                    $dinvlr2015 = $dados['dinvlr2015'];
                    echo campo_texto('dinvlr2015', 'N', "S", 'Modelo', 30, 26, '###.###.###.###.###.###,##', '', '', '', '', 'id="dinvlr2015"', '', $dinvlr2015);
                ?>
            </td>
            <td width="75%"> <b>2016</b> <br>
                <?PHP
                    $dinvlr2016 = $dados['dinvlr2016'];
                    echo campo_texto('dinvlr2016', 'N', "S", 'Modelo', 30, 26, '###.###.###.###.###.###,##', '', '', '', '', 'id="dinvlr2016"', '', $dinvlr2016);
                ?>
                
                <br><br> <b>2017</b> <br>
                
                <?PHP
                    $dinvlr2017 = $dados['dinvlr2017'];
                    echo campo_texto('dinvlr2017', 'N', "S", 'Modelo', 30, 26, '###.###.###.###.###.###,##', '', '', '', '', 'id="dinvlr2017"', '', $dinvlr2017);
                ?>
            </td>
        </tr>
        <tr>
            <td colspan="5" class="subTituloCentro">
                <input class="salvarDadosDemanda" type="button" value="Salvar" onclick="salvarDadosDemanda();" style="cursor:pointer">
                <input type="button" value="Fechar" style="cursor:pointer" onclick="javascript: window.opener.location.reload();">
            </td>
        </tr>
    </table>

    <br>
<?PHP if($dinid != ''){ ?>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubtituloDireita">Arquivo:</td>
            <td>
                <input type="file" name="arquivo" id="arquivo">
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Descrição:</td>
            <td>
                <?= campo_textarea('arqdescricao', 'S', 'S', '', 80, 3, 250, '', 0, '', false, NULL, $arqdescricao) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Tipo:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  tpaid as codigo,
                                tpadsc as descricao
                        FROM academico.tipoarquivo
                    ";
                    $db->monta_combo('tpaid_arquivo', $sql, 'S', 'Selecione', $acao, $opc, '', '200', 'S', 'tpaid_arquivo', false, $tpaid_arquivo, null);
                ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="subTituloCentro">
                <input class="anexar" onclick="anexarArquivos();" type="button" value="Anexar" style="cursor:pointer">
            </td>
        </tr>
    </table>
</form>

<br>

<?PHP
        monta_titulo('Lista de Arquivos','');

        $acao = "
            <center>
                <img src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor: pointer\" onclick=\"excluirArquivoInfra('|| a.arqid ||')\" id=\"'|| a.arqid ||'\">
                &nbsp;&nbsp;
                <img src=\"../imagens/indicador-vermelha.png\" title=\"Baixar Arquivo (Download)\" style=\"cursor: pointer\" onclick=\"downloadArquivoInfra('|| a.arqid ||')\" id=\"'|| a.arqid ||'\">
            </center>
        ";

        $sql = "
            SELECT '{$acao}' as acao,
                    ta.tpadsc as tipo_doc,
                    arqdescricao as descricao,
                    arqnome||'.'||arqextensao as arqnome,
                    arqtamanho as tamanho,
                    to_char(arqdata, 'DD/MM/YYYY') as data
            FROM academico.demandainfra di
            JOIN academico.arquivotipologia AS a ON a.dinid = di.dinid
            JOIN academico.tipoarquivo AS ta ON ta.tpaid = a.tpaid
            JOIN public.arquivo AS arq ON arq.arqid = a.arqid
            WHERE di.dinid  = {$dinid}
        ";
        $cabecalho = array("<center>Ação</center>", "Tido de Documento", "Descrição Arquivo", "Nome Arquivo", "Tamanho (MB)", "Data Inclusão");
        $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'left');
        $tamanho = Array('5%', '10%', '50%', '10%', '10%', '10%');
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
    }
?>