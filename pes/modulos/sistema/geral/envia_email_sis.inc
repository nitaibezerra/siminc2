<?php
# carrega as bibliotecas
include_once APPRAIZ . "includes/PogProgressBar.php";
include_once APPRAIZ . "includes/envia_email_sis_geral_funcoes.inc";
include_once APPRAIZ . "includes/Agrupador.php";
if ($_REQUEST['req'] == 'exibirDestinatarios') {

    $_SESSION['municipios'] = $_REQUEST['arMunicipio'] ? substr($_REQUEST['arMunicipio'], 0, -2) : '';
    die();
}

switch ($_SESSION['sisarquivo']) {
    case 'cte':

        $_REQUEST['statusUsuario'] = ($_REQUEST['statusUsuario'] ) ? $_REQUEST['statusUsuario'] : 'A';

        $suscod = $_REQUEST['statusUsuario'] ? " us.suscod = '" . $_REQUEST['statusUsuario'] . "' and " : "";
        $exibefiltromunicipios = true; // Mostra o filtro por municipios

        if ($_REQUEST['filtromun'] == 'sim' && $_REQUEST['perfil'][0]) {
            if ($_REQUEST['perfil'] && $_REQUEST['municipios']) {

                // carregando os perfis escolhidos que possam ser filtrados por municipio
                $perfismun = $db->carregar("SELECT * FROM cte.tprperfil WHERE pflcod IN('" . implode("','", $_REQUEST['perfil']) . "') AND tprcod = '2'");
                // se algum dos perfis selecionados pode ser filtrado por municipio
                if ($perfismun) {
                    $_REQUEST['perfil'] = array_flip($_REQUEST['perfil']);

                    $joinIdeb = $_REQUEST['ideb'] ? " INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = ur.muncod " : "";
                    $clausulaIdeb = $_REQUEST['ideb'] ? " AND mtm.tpmid IN ( " . implode(", ", $_REQUEST['ideb']) . " ) " : "";

                    foreach ($perfismun as $per) {
                        $sql = "SELECT distinct u.usuemail, u.usunome FROM cte.usuarioresponsabilidade ur
								LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf
								INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf
								$joinIdeb
								WHERE $suscod
								ur.pflcod = " . $per['pflcod'] . "
								AND us.sisid = " . $_SESSION["sisid"] . "
								AND rpustatus = 'A'
								AND ur.muncod IN('" . implode("','", $_REQUEST['municipios']) . "')
								$clausulaIdeb";

                        $desti = $db->carregar($sql);
                        // se existe algum usuario desses filtros, processar na opção de outros
                        if ($desti) {
                            foreach ($desti AS $de) {
                                $_REQUEST["pessoas"] .= $de['usunome'] . " <" . $de['usuemail'] . ">,";
                            }
                        }
                        unset($_REQUEST['perfil'][$per['pflcod']]);
                    }
                    $_REQUEST['perfil'] = array_flip($_REQUEST['perfil']);
                }
            }
        }
        break;
}


//print_r($_REQUEST);
if ($_REQUEST["formulario"] == 2) {

    $orgao = K_ORGCODIGO;
    $perfis = (array) $_REQUEST["perfil"];
    $status = $_REQUEST["status"];

    $outros = $_REQUEST["pessoas"];

    // Montando o AND quando tiver UO na pesquisa
    if($_REQUEST['entidade'] && !empty($_REQUEST['entidade'][0])){
        $andEntidade = 'AND e.entcodigo IN (\'' . implode( "' , '", $_REQUEST['entidade'] ) . '\')';
    } else
        $andEntidade = '';

    $teste = array(array(), array());

    // Montando o AND de perfis na pesquisa.
    if($_REQUEST['perfil'] && !empty($_REQUEST['perfil'][0])){
        $andPerfil = 'AND urp.pflcod IN (\'' . implode( "' , '", $perfis ) . '\')';
    } else
        $andPerfil = '';

    $sql = "SELECT DISTINCT u.usunome , u.usuemail , u.usucpf
            FROM  pes.pesunidadeorcamentaria uo
                INNER JOIN pes.pesentidade e on uo.uorcodigo = e.uorcodigo
                INNER JOIN pes.usuarioresponsabilidade urp ON urp.entcodigo = e.entcodigo
                INNER join seguranca.perfilusuario pu on pu.usucpf = urp.usucpf AND rpustatus = 'A'
    --            INNER join seguranca.perfil p on p.pflcod = pu.pflcod and p.pflstatus = 'A'
                INNER JOIN seguranca.usuario u  on urp.usucpf = u.usucpf
            WHERE uo.orgcodigo = '{$orgao}'
            AND urp.rpustatus = 'A'
            {$andEntidade}
            {$andPerfil}
--            AND o.aexano = " . AEXANO . "
--            AND uo.aexano = " . AEXANO . "
--            AND e.aexano = " . AEXANO;

    if (current($_REQUEST['status'])) {

        $anoInicial = 2013;
        $anoFinal   = date('Y');
        $mesAtual   = date('m') == 1 ? 12 : (date('m') - 1);

        $sWhere  = current($_REQUEST['entidade']) ? ' and ent.entcodigo in (' . implode(', ', $_REQUEST['entidade']) . ')' : ' ';
        $sWhere .= current($_REQUEST['perfil']) ? ' and rpu.pflcod in (' . implode(', ', $_REQUEST['perfil']) . ')' : ' ';

        $sql = "select distinct usunome , usuemail , usucpf
                from (
                    select  distinct ent.entcodigo, ent.uorcodigo, u.usunome , u.usuemail , u.usucpf, rpu.pflcod
                    from pes.pesentidade ent
                        inner join pes.pesunidadeorcamentaria uor on uor.uorcodigo = ent.uorcodigo
                        inner join pes.usuarioresponsabilidade rpu on rpu.entcodigo = ent.entcodigo and rpustatus = 'A'
                        INNER join seguranca.perfilusuario pu on pu.usucpf = rpu.usucpf AND rpustatus = 'A'
                        INNER JOIN seguranca.usuario u  on rpu.usucpf = u.usucpf
                        left  join (
                        select entcodigo, ano, sum(total) as total
                        from(
                        select cont.entcodigo, ceaano as ano, sum(cea.ceavalor) as total
                        from pes.pescontrato cont
                            inner join pes.pescelulaacompanhamento  cea on cea.concodigo = cont.concodigo
                            inner join pes.pesconfigcontratodespesa ccd on ccd.ccdcodigo = cea.ccdcodigo
                            inner join pes.pescolunacontrato cco on cco.ccocodigo = ccd.ccocodigo
                        where ceaano  >= $anoFinal
                        and ccototaliza = 'S'
                        group by cont.entcodigo, ceaano

                        union all

                        select cnd.entcodigo, canano as ano, sum(can.canvalor) as total
                        from pes.pescelulaacompnatdespesa can
                            inner join pes.pescontratonaturezadespesa cnd on cnd.cndcodigo = can.cndcodigo
                        where canano  >= $anoFinal
                        and cantipovalor = 'FN'
                        group by cnd.entcodigo, canano
                        ) as agrupador
                        group by entcodigo, ano
                        ) con on con.entcodigo = ent.entcodigo
                        left join (
                        select entcodigo, -- sum(naoiniciado) as naoiniciado, sum(aprovado) as aprovado, sum(reprovado) as reprovado,
                           -- (select count(*) from pes.pestipodespesa where tidacompanha = 't' and tidpossuimeta = 't') * $mesAtual as totalmaximo,
                           case
                          when sum(aprovado) >= ((select count(*) from pes.pestipodespesa where tidacompanha = 't' and tidpossuimeta = 't') * $mesAtual) then '4'
                          when (sum(aprovado) + sum(reprovado)) > 0 then '3'
                          else '2'
                           end as status
                        from (
                        select entcodigo, count(*) as naoiniciado, 0 as aprovado, 0 as reprovado
                        from pes.pesvalidacao
                        where  valstatus = 'NI'
                        and valmes <= $mesAtual
                        and valano = $anoFinal
                        group by entcodigo, aprovado,reprovado

                        union all

                        select entcodigo, 0 as naoiniciado, count(*) as aprovado, 0 as reprovado
                        from pes.pesvalidacao
                        where  valstatus = 'APR'
                        and valmes <= $mesAtual
                        and valano = $anoFinal
                        group by entcodigo, naoiniciado,reprovado

                        union all

                        select entcodigo, 0 as naoiniciado, 0 as aprovado, count(*) as reprovado
                        from pes.pesvalidacao
                        where  valstatus = 'REP'
                        and valmes <= $mesAtual
                        and valano = $anoFinal
                        group by entcodigo, naoiniciado,aprovado
                        ) as validacao
                        group by entcodigo
                        ) as val on val.entcodigo = ent.entcodigo
                    where orgcodigo = '26000'
                    $sWhere
                    and  (   case
                            when coalesce(val.status, '') = '' and coalesce(total, 0) = 0 then '1'
                            when coalesce(val.status, '') = '' and coalesce(total, 0) > 0 then '2'
                            else val.status
                        end ) in ('" . implode("', '", $_REQUEST['status']) . "')

                ) as tabela_status
                ";
    }

    $destinatarios = $db->carregar($sql);

    if($outros){
        if(!$destinatarios) $destinatarios =array();
        $destinatarios = EmailSistema::identificar_outros_destinatarios($destinatarios,$outros);
    }

//    foreach($destinatarios as &$desinatario)
//        $desinatario = $desinatario['usucpf'];

    if (empty($destinatarios)) {
        echo '<script type="text/javascript">alert( "Não há destinatários para os filtros indicados." )</script>';
    //-----------------adiciona o remetente do e-mail à lista de destinatários
    } else {

        if( $_REQUEST["stNomeRemetente"] && $_REQUEST["stEmailRemetente"] ){
		$remetente['usunome'] = $_REQUEST["stNomeRemetente"];
		$remetente['usuemail'] = $_REQUEST["stEmailRemetente"];
		$remetente['usucpf'] = "";
	} else {
		$sql = "select distinct u.usunome , u.usuemail, u.usucpf
				from seguranca.usuario u
				where u.usucpf = '".$_SESSION['usucpforigem']."'
			    group by u.usunome, u.usuemail, u.usucpf ";

		$remetente = $db->pegaLinha( $sql );
	}

//        $remetente = array('nome' => $_SESSION['usunome'] , 'email' => $_SESSION['usuemail']);
        if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
            $destinatarios = array();
            $destinatarios[] = array('usunome' => 'RUY JUNIOR FERREIRA SILVA' ,
                                    'usuemail' => 'ruyjfs@gmail.com',
                                    'usucpf' => ''
                                    );
            $destinatarios[] = array('usunome' => 'ORION' ,
                                    'usuemail' => 'orion.mesquita@mec.gov.br',
                                    'usucpf' => ''
                                    );
        }

        if(!in_array($remetente, $destinatarios)){
            array_push($destinatarios, $remetente);
        }
    }

    // compila os dados da mensagem
    $assunto = $_REQUEST["assunto"];
    $conteudo = $_REQUEST["mensagem"];

    if ($_FILES['anexo']['error'] == 0) {
        $anexos = array();
        $anexos[] = array('arquivo' => $_FILES['anexo']['tmp_name'],  'nome' => $_FILES['anexo']['name']);
    } else $anexos = array();

    #tratando o remetendo para funcao enviar_email
    $remetente = array('nome'=> $remetente['usunome'], 'email' => $remetente['usuemail']);

    # envia as mensagens
    //----------------------------------------------------------------------------------
    $result = enviar_email( $remetente, $destinatarios, $assunto, $conteudo,  null, 'orion.mesquita@mec.gov.br', $anexos);

//    ver($remetente, $destinatarios, $assunto, $conteudo,  null, array('ruyjfs@gmail.com'), $anexos,d);
    if ( $result ) {
//        ver('DEU' , $destinatarios, $assunto, $conteudo, $_FILES, $remetente,d);
        $db->commit();
        $db->sucesso( $_REQUEST["modulo"] );
    }
//        ver('NÃO DEU', $destinatarios, $assunto, $conteudo, $_FILES, $remetente, d);
        $db->rollback();
        $db->insucesso( "Ocorreu uma falha ao enviar a mensagem." );
}

include APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
monta_titulo($titulo_modulo, "");

?>

<!-- AUTO COMPLEMENTO -->
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/keys/keyEvents.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/tags/suggest.js"></script>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<link href="css/jquery-ui/custom-theme/jquery-ui-1.10.3.custom.css" rel="stylesheet">
<script src="js/jquery-1.9.1.js"></script>
<script src="js/jquery-ui-1.10.3.custom.js"></script>
<link href="css/estilo.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/funcoes.js"></script>


<style>
    .suggestMarked
    {
        background-color: #AAAAFF;
        width:500px;
        border-style: solid;
        border-width: 1px;
        border-color: #DDDDFF;
        border-top-width: 0px;
        position: relative;
        z-index: 100;
    }
    .suggestUnmarked
    {
        background-color: #EEEEFF;
        width:500px;
        border-style: solid;
        border-width: 1px;
        border-color: #DDDDFF;
        border-top-width: 0px;
        z-index: 100;
        position: relative;
    }

    fieldset{
        width: 475px;
        padding: 10px;
    }

    fieldset input{
        width: 150px;
    }
    fieldset label{
        display: block;
        width: 40px;
        float: left;
    }

    fieldset br{
        clear: both;
    }

</style>

<!-- EDITOR DE TEXTO -->
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<script language="javascript" type="text/javascript">
    tinyMCE.init({
        mode: "specific_textareas",
        textarea_trigger: "mce_editable",
        theme: "advanced",
        plugins: "table,save,advhr,advimage,advlink,emotions,iespell,insertdatetime,preview,zoom,flash,searchreplace,print,contextmenu,paste,directionality,fullscreen",
        theme_advanced_buttons1: "undo,redo,separator,bold,italic,underline,forecolor,backcolor,fontsizeselect,separator,justifyleft,justifycenter,justifyright, justifyfull, separator, outdent,indent, separator, bullist, code",
        theme_advanced_buttons2: "",
        theme_advanced_buttons3: "",
        theme_advanced_toolbar_location: "top",
        theme_advanced_toolbar_align: "left",
        extended_valid_elements: "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]",
        language: "pt_br",
        entity_encoding: "raw"
    });
</script>

<!-- EVENTOS DO FORMULÁRIO -->
<script language="javascript" type="text/javascript">
    function abrepopupMunicipio() {
        window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/cte/combo_municipios_enviaemail.php', 'Municipios', 'width=400,height=400,scrollbars=1');
    }

    function abrepopupMunicipioUsuario() {
        window.open('http://<?= $_SERVER['SERVER_NAME'] ?>/geral/combo_municipios_enviaemail.php', 'Municipios', 'width=400,height=400,scrollbars=1');
    }

    function exibir_destinatarios() {
        // captura os campos
        var campo_orgao = document.getElementById("orgao");
        var campo_uo = document.getElementById("entidade");
        var campo_ug = document.getElementById("unidadegestora");
        var campo_perfis = document.getElementById("perfil");
        var campo_municipios = document.getElementById("municipios");
        var campo_outros = document.getElementById("pessoas");
        var obStatusUsuario = document.getElementById("statusUsuario");
        var obIDEB = document.getElementById("ideb");
        // valores dos campos capturados
        var orgao = "";
        var uo = new Array();
        var ug = "";
        var perfis = new Array();
        var municipios = new Array();
        var arIDEB = new Array();
        var outros = "";
        // identifica o órgão selecionado
        if (campo_orgao) {
            if (campo_orgao.selectedIndex != 0) {
                orgao = campo_orgao.options[campo_orgao.selectedIndex].value;
            }
        }
        if (orgao) {
            // captura as unidades orçamentárias
            if (campo_uo) {
                for (i = 0; campo_uo.options[i]; i++) {
                    if (campo_uo.options[i].value) {
                        uo.push(campo_uo.options[i].value);
                    }
                }
            }
            // captura as unidades gestoras
            if (campo_ug) {
                ug = campo_ug.options[campo_ug.selectedIndex].value;
            }
        }
        // captura os perfis
        if (campo_perfis) {
            for (i = 0; campo_perfis.options[i]; i++) {
                if (campo_perfis.options[i].value) {
                    perfis.push(campo_perfis.options[i].value);
                }
            }
        }

        // captura IDEB
        if (obIDEB) {
            for (i = 0; obIDEB.options[i]; i++) {
                if (obIDEB.options[i].value) {
                    arIDEB.push(obIDEB.options[i].value);
                }
            }
        }

        // captura os municipios
        if (campo_municipios) {
            for (i = 0; campo_municipios.options[i]; i++) {
                if (campo_municipios.options[i].value) {
                    municipios.push(campo_municipios.options[i].value);
                }
            }
        }

        // captura outros destinatários
        if (campo_outros) {
            outros = campo_outros.value;
        }

        // captura Status dos Usuários
        statusUsuario = obStatusUsuario.value;

        var parametros = "";
        parametros += "?pessoas=" + escape(outros);

        parametros += "&statusUsuario=" + statusUsuario;
        if (orgao) {
            parametros += "&orgao=" + orgao;
            if (uo) {
                for (i = 0; uo[i]; i++) {
                    parametros += "&entidade[]=" + uo[i];
                }
            }
            if (ug) {
                parametros += "&unidadegestora=" + ug;
            }
        }
        if (perfis) {
            for (i = 0; perfis[i]; i++) {
                parametros += "&perfil[]=" + perfis[i];
            }
        }

        if (arIDEB) {
            for (i = 0; arIDEB[i]; i++) {
                parametros += "&ideb[]=" + arIDEB[i];
            }
        }

        var arMunicipio = "";
        if (municipios) {
            for (i = 0; municipios[i]; i++) {
                arMunicipio += "'" + municipios[i] + "', ";
            }
        }

<?
if ($exibefiltromunicipios) {
    ?>
            if (document.getElementById("filtromunsim").checked) {
                parametros += '&filtromun=sim';
            } else {
                parametros += '&filtromun=nao';
            }
<? } ?>

        /*if(document.getElementById("filtromunsim").checked) {
         parametros += '&filtromun=sim';
         } else {
         parametros += '&filtromun=nao';
         }*/


        return new Ajax.Request(window.location.href, {
            method: 'post',
            parameters: {
                req: 'exibirDestinatarios',
                parametros: parametros,
                arMunicipio: arMunicipio
            },
            onComplete: function(res) {
                window.open("/geral/destinatarios.php" + parametros, "Destinatários", "resizable=no,scrollbars=yes,status=no,width=640,height=480");
            }
        });

    }
</script>

<!-- FORMULÁRIO -->
<script language="javascript" type="text/javascript">
    function submeter_formulario() {
        
        
//        console.info($('#mensagem'));
//        console.info($('#mensagem').val());
//        console.info(tinyMCE.get('mensagem').getContent());
//        console.info(tinyMCE.get('#mensagem').getContent());
        
        
        if($('#assunto').val() == ''){
            msg($('#assunto'), "O campo 'Assunto' não pode ser vazio!");
            $('#assunto').focus();
            return false;
        }
        
//        if($('#mensagem').val() == ''){
//            msg($('#mensagem'), "O campo 'Mensagem' não pode ser vazio!");
//            $('#assunto').focus();
//            return false;
//        }
        
        
        var formulario = document.getElementById('formulario');
        formulario.formulario.value = 2;
        var unidades_orcamentarias = formulario.entidade;
        if (unidades_orcamentarias) {
            if (unidades_orcamentarias.options[0].value == "") {

            }
            selectAllOptions(unidades_orcamentarias);
        }

        //var estuf = formulario.estuf;
        var perfil = formulario.perfil;
        var municipiosUsuario = formulario.municipiosUsuario;

        if (perfil) {
            selectAllOptions(perfil);
        }

        var status = formulario.status;
        if (status) {
            selectAllOptions(status);
        }

        if (municipiosUsuario) {
            selectAllOptions(municipiosUsuario);
        }
        /*
         if ( estuf ) {
         selectAllOptions( estuf );
         }
         */
        selectAllOptions(document.getElementById('ideb'));
        formulario.submit();
    }
</script>
<form action="" id="formulario" method="post" name="formulario" enctype="multipart/form-data">
    <input type="hidden" name="formulario" value="2"/>
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <colgroup>
            <col style="width: 25%;">
            <col style="width: 75%;">
        </colgroup>
        <tbody>
            <tr>
                <td align='right' class="SubTituloDireita">Unidade Orçamentária:</td>
                <td>
                    <?php
                        $orgcodigo = K_ORGCODIGO;
                        $sqlUOUsuarios = "SELECT DISTINCT e.entcodigo as codigo , uo.uorcodigo || ' - ' || e.entnome as descricao
                                FROM pes.pesorgao o
                                LEFT JOIN pes.pesunidadeorcamentaria uo on uo.orgcodigo = o.orgcodigo
                                LEFT JOIN pes.pesentidade e on uo.uorcodigo = e.uorcodigo
                                RIGHT JOIN pes.usuarioresponsabilidade urp ON urp.entcodigo = e.entcodigo
                                WHERE o.orgcodigo = '{$orgcodigo}'
                                AND rpustatus = 'A'
                                AND o.aexano = " . AEXANO . "
                                AND uo.aexano = " . AEXANO . "
                                AND e.aexano = " . AEXANO;
                        combo_popup( 'entidade', $sqlUOUsuarios, 'Selecione a(s) Entidadee(s)', '400x400', 0, array(), '', 'S', false, false, 10, 500, '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Perfil:</td>
                <td>
                    <?php
                    $sql =
                            "select pflcod as codigo, pfldsc as descricao
						from perfil
						where sisid = '{$_SESSION["sisid"]}' and pflstatus = 'A'
						order by pfldsc";

                    combo_popup('perfil', $sql, 'Selecione os Perfis', '400x400', 0, array(), '', 'S', false, false, 10, 500, '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Situação:</td>
                <td>
                    <?php

                        $sqlSituacao = "select descricao, codigo
                                        from (
                                            select 'Cadastramento não iniciado' as descricao, 1 as codigo
                                            union
                                            select 'Em cadastramento', 2
                                            union
                                            select 'Em validação', 3
                                            union
                                            select '100% Validado', 4
                                        ) as status
                                        order by codigo";

//                        $arrSituacao = array();
//                        $arrSituacao[] = array('codigo' => 1, 'descricao' => 'Cadastramento não iniciado');
//                        $arrSituacao[] = array('codigo' => 2, 'descricao' => 'Em cadastramento');
//                        $arrSituacao[] = array('codigo' => 3, 'descricao' => 'Em validação');
//                        $arrSituacao[] = array('codigo' => 4, 'descricao' => 'Aprovado');
                        combo_popup('status', $sqlSituacao, 'Selecione os Perfis', '400x400', 0, array(), '', 'S', false, false, 10, 500, '', '');
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Outros Destinatários:</td>
                <td>
                    <textarea type="text" name="pessoas" id="pessoas" style="width: 500px; height: 100px" ><?= $_REQUEST["pessoas"] ?></textarea>
                    <br/>
                    <div id="divSuggestPessoas" style="position: absolute; z-index: 1000;"></div>
                    <script>
                        objSuggestPessoas = new window.Suggest(
                                document.getElementById("pessoas"),
                                document.getElementById("divSuggestPessoas"),
                                "suggestlist.php"
                                );
                    </script>
                    <br />
                    <strong>Obs.</strong> Separe os emails sómente com "," (vírgula)
                    <br />
                    <strong>Exemplo:</strong> contato@mec.gov.br , contato2@mec.gov.br
                </td>
            </tr>
            <tr>
                <td colspan="2" style="text-align: center;" class="subtitulodireita">&nbsp;</td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Assunto:</td>
                <td><input class="normal" type="text" value="<?= $_REQUEST["assunto"] ?>" id="assunto"  name="assunto" style="width: 500px" /><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Anexo:</td>
                <td><input type="file" name="anexo" /></td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Mensagem:</td>
                <td>
                    <textarea mce_editable="true" id="mensagem"  name="mensagem" style="width: 500px; height: 150px"><?= $_REQUEST["mensagem"] ?></textarea>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita">Remetente:</td>
                <td>
                    <fieldset>
                        <legend>Dados do Remetente</legend>
                        <label for="stNomeRemetente">Nome: </label><input class="normal" type="text" name="stNomeRemetente" id="stNomeRemetente" /><br />
                        <label for="stEmailRemetente">E-Mail: </label><input class="normal" type="text" name="stEmailRemetente" id="stEmailRemetente" /><br /><br />
                        * No caso do não preenchimento dos campos acima, o destinatário será o usuário logado no sistema.
                    </fieldset>
                </td>
            </tr>
            <tr bgcolor="#C0C0C0">
                <td></td>
                <td>
                    <input type='button' class="botao" name='EnviarEmail' value='Enviar' onclick="submeter_formulario();"/>
                    <!-- span style="cursor: pointer; color:#FF0000;" onclick="exibir_destinatarios();">Clique aqui para verificar a lista de destinátarios.</span -->
                </td>
            </tr>
        </tbody>
    </table>
</form>