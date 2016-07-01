<?php
include APPRAIZ . "includes/cabecalho.inc";
echo '<br>';
monta_titulo('Gestão de Pessoas', 'Força de trabalho');

//segurança e filtro para perfil Gestor e AIDP
//verificaPerfilGestoreAidp();

$abacod_tela    = ABA_FORCA_TRAB_MENU_INICIAL;
$url            = 'gestaopessoa.php?modulo=principal/forca_trabalho/painel_forca_trabalho&acao=A';
$parametros     = '';

$db->cria_aba($abacod_tela, $url, $parametros);


?>

<script src="../includes/prototype.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <?php if (controlaPermissao('administrador') || controlaPermissao('consulta') || controlaPermissao('superuser')) { ?>
            <td>
                <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
                    <form id="formulario" name="formulario" method="post" action="?modulo=principal/listaPessoalAvaliacao&acao=A" >
                        <input type="hidden" name="filtro_auto_aval" id="filtro_auto_aval" value="" >
                        <input type="hidden" name="filtro_aval_superior" id="filtro_aval_superior" value="" >
                        <input type="hidden" name="filtro_consenso" id="filtro_consenso" value="" >
                        <input type="hidden" name="filtro_media" id="filtro_media" value = "">
                    </form>
                    <tr>
                        <td colspan="2">
                            <b>Geral</b>
                        </td>
                    </tr>
                    <tr>
                        <td>  Total de Servidores </td>
                        <?php
                            $sqlb = "
                                SELECT count( fdp.fdpcpf )
                                FROM gestaopessoa.ftdadopessoal as fdp
                                LEFT JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
                                LEFT JOIN gestaopessoa.ftdadofuncional ful ON ful.fdpcpf = fdp.fdpcpf
                                WHERE fdp.fdpcpf IS NOT NULL AND fst.fstid <> 4
                                " . ($filtroGestor ? " AND fst.fstid in (" . $filtroGestor . ") " : '' ) . "
                                " . ($filtroAidp ? " AND ful.fulid in (" . $filtroAidp . ") " : '' ) . "
                            ";
                        ?>
                        <td style="color:rgb(0, 102, 204);text-align:right"><?= $db->pegaUm($sqlb); ?></td>
                    </tr>

                    <tr>
                        <td colspan="2">
                            <img id="img_unidade" onclick="onOff('unidade');" src="../imagens/mais.gif">&nbsp;<b>Total de servidores X Unidade de Lotação</b>
                        </td>
                    </tr>
                    <tr id="unidade" style="display:none;">
                        <td colspan="2">
                            <?php
                                $sql = "SELECT * FROM gestaopessoa.ftunidadelotacao";
                                $rs = $db->carregar($sql);
                                $rs = $rs? $rs: array();
                                foreach ($rs as $i => $tp){
                                    if(($i%2) == 0){
                                        $marcado='#DCDCDC';
                                    }else{
                                        $marcado = '' ;
                                    }
                            ?>
                                    <table width="100%">
                                        <tr style="width:100%; background-color:<?=$marcado;?>;">
                                            <td style="width:98%; text-align: left;">
                                                - <a href="javascript: listaPessoalFiltro('filtro_ftu_fulid', <?= $tp['fulid']; ?>);"> <?= $tp['fuldescricao']; ?> </a>
                                            </td>
                                            <td style="width:2%; text-align: right; font-style:oblique; font-weight: bold;">
                                                <?php
                                                    $sqlb = "
                                                        SELECT count(x.contagem) FROM
                                                        (SELECT ful.fdpcpf as contagem
                                                        FROM gestaopessoa.ftdadopessoal as fdp
                                                        LEFT JOIN gestaopessoa.ftdadofuncional as ful ON fdp.fdpcpf = ful.fdpcpf
                                                        LEFT JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
                                                        WHERE fdp.fdpcpf IS NOT NULL AND fst.fstid <> 4
                                                        AND fulid = {$tp['fulid']}
                                                        " . ($filtroGestor ? " AND fst.fstid in (" . $filtroGestor . ") " : '' ) . "
                                                        " . ($filtroAidp ? " AND ful.fulid in (" . $filtroAidp . ") " : '' ) . "
                                                                                                      ) AS x
                                                    ";
                                                    echo ($db->pegaUm($sqlb) ? $db->pegaUm($sqlb) : 0);
                                                ?>
                                            </td>
                                        </tr>
                                    </table>
                            <? } ?>
                        </td>
                    </tr>

                    <tr>
                        <td colspan="2">
                            <img id="img_situacao" onclick="onOff('situacao');"src="../imagens/mais.gif">&nbsp;<b>Dados Funcionais por Situação no MEC</b>
                        </td>
                    </tr>
                    <tr id="situacao" style="display: none;">
                        <td colspan="2">
                            <?php
                                $sql = "SELECT * FROM gestaopessoa.ftsituacaotrabalhador WHERE fstid <> 4";
                                $rs = $db->carregar($sql);
                                $rs = $rs? $rs: array();
                                foreach ($rs as $i => $tp) {
                                    if(($i%2) == 0){
                                        $marcado='#DCDCDC';
                                    }else{
                                        $marcado = '' ;
                                    }
                            ?>
                                    <table width="100%">
                                        <tr style="width:100%; background-color:<?=$marcado;?>;">
                                            <td style="width:98%; text-align: left;">
                                                - <a href="javascript: listaPessoalFiltro('filtro_fst_fstid', <?= $tp['fstid']; ?>);"><?= $tp['fstdescricao']; ?></a>
                                            </td>
                                            <td style="width:2%; text-align: right; font-style:oblique; font-weight: bold;">
                                                <?php
                                                    $sqlb = "
                                                        SELECT count( fdp.fdpcpf )
                                                        FROM gestaopessoa.ftdadopessoal as fdp
                                                        LEFT JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
                                                        LEFT JOIN gestaopessoa.ftdadofuncional ful ON ful.fdpcpf = fdp.fdpcpf
                                                        WHERE fdp.fdpcpf IS NOT NULL
                                                        " . ($filtroGestor ? " AND fst.fstid in (" . $filtroGestor . ") " : '' ) . "
                                                        " . ($filtroAidp ? " AND ful.fulid in (" . $filtroAidp . ") " : '' ) . "
                                                        AND fst.fstid = " . $tp['fstid'];
                                                echo $db->pegaUm($sqlb);
                                    ?>
                                            </td>
                                        </tr>
                                    </table>
                            <? } ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <br><b>Formação Acadêmica</b>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <img id="img_formacao" onclick="onOff('formacao');" src="../imagens/mais.gif">&nbsp;<b>Formação Acadêmica X Grau de Escolaridade</b>
                        </td>
                    </tr>
                    <tr id="formacao" style="display: none;">
                        <td colspan="2">
                            <?php
                                $sql = "SELECT  tfoid, tfodsc from public.tipoformacao where tfoid not in (5,6,7)";
                                $rs = $db->carregar($sql);
                                $rs = $rs? $rs: array();
                                foreach ($rs as $i => $tp) {
                                    if(($i%2) == 0){
                                        $marcado='#DCDCDC';
                                    }else{
                                        $marcado = '' ;
                                    }
                            ?>
                                    <table width="100%">
                                        <tr style="width:100%; background-color:<?=$marcado;?>;">
                                            <td style="width:98%; text-align: left;">
                                                - <a href="javascript: listaPessoalFiltro('filtro_tfo_tfoid', <?= $tp['tfoid']; ?>);"><?= $tp['tfodsc']; ?></a>
                                            </td>
                                            <td style="width:2%; text-align: right; font-style:oblique; font-weight: bold;">
                                                <?php
                                                    $sqlb = "
                                                        SELECT count(x.contagem) FROM
                                                        (SELECT distinct fdp.fdpcpf as contagem
                                                        FROM gestaopessoa.ftdadopessoal as fdp
                                                        LEFT JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
                                                        LEFT JOIN gestaopessoa.ftdadofuncional ful ON ful.fdpcpf = fdp.fdpcpf
                                                        LEFT  JOIN gestaopessoa.ftformacaoacademica as formacao ON fdp.fdpcpf = formacao.fdpcpf
                                                        WHERE fdp.fdpcpf IS NOT NULL
                                                        AND fst.fstid <> 4
                                                        " . ($filtroGestor ? " AND fst.fstid in (" . $filtroGestor . ") " : '' ) . "
                                                        " . ($filtroAidp ? " AND ful.fulid in (" . $filtroAidp . ") " : '' ) . "
                                                        AND formacao.tfoid = " . $tp['tfoid'] . "
                                                        ) AS x
                                                   ";
                                                    echo $db->pegaUm($sqlb);
                                                ?>
                                            </td>
                                        </tr>
                                    </table>
                            <? } ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <img id="img_idioma" onclick="onOff('idioma');" src="../imagens/mais.gif">&nbsp;<b>Idioma</b>
                        </td>
                    </tr>
                    <tr id="idioma" style="display: none;">
                        <td colspan="2">
                            <?php
                                $sql = "SELECT * FROM gestaopessoa.ftitipoidioma";
                                $rs = $db->carregar($sql);
                                $rs = $rs? $rs: array();
                                foreach ($rs as $i => $tp){
                                    if(($i%2) == 0){
                                        $marcado='#DCDCDC';
                                    }else{
                                        $marcado = '' ;
                                    }
                            ?>
                                    <table width="100%">
                                        <tr style="width:100%; background-color:<?=$marcado;?>;">
                                            <td style="width:98%; text-align: left;">
                                                - <a href="javascript: listaPessoalFiltro('filtro_fti_ftiid', <?= $tp['ftiid']; ?>);"><?= $tp['ftidescricao']; ?></a>
                                            </td>
                                            <td style="width:2%; text-align: right; font-style:oblique; font-weight: bold;">
                                                <?PHP
                                                    $sqlb = "
                                                        SELECT count(x.contagem) FROM
                                                        ( SELECT distinct idioma.fdpcpf as contagem
                                                        FROM gestaopessoa.ftdadopessoal as fdp
                                                        LEFT JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
                                                        LEFT JOIN gestaopessoa.ftdadofuncional ful ON ful.fdpcpf = fdp.fdpcpf
                                                        LEFT  JOIN gestaopessoa.idioma as idioma ON fdp.fdpcpf = idioma.fdpcpf
                                                        WHERE fdp.fdpcpf IS NOT NULL
                                                        AND fst.fstid <> 4
                                                        " . ($filtroGestor ? " AND fst.fstid in (" . $filtroGestor . ") " : '' ) . "
                                                        " . ($filtroAidp ? " AND ful.fulid in (" . $filtroAidp . ") " : '' ) . "
                                                        AND idioma.ftiid = " . $tp['ftiid'] . "
                                                        ) AS x
                                                    ";
                                                    echo $db->pegaUm($sqlb);
                                                ?>
                                            </td>
                                        </tr>
                                    </table>
                            <? } ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <BR><b>Atividade Desenvolvida</b>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <img id="img_atividade" onclick="onOff('atividade');" src="../imagens/mais.gif">&nbsp;<b>Atividade Desenvolvida X Tipo</b>
                        </td>
                    </tr>
                    <tr id="atividade" style="display: none;">
                        <td colspan="2">
                            <?php
                                $sql = "SELECT * FROM gestaopessoa.fttipoatividadedesenvolvida";
                                $rs = $db->carregar($sql);
                                $rs = $rs? $rs: array();
                                foreach ($rs as $i => $tp){
                                    if(($i%2) == 0){
                                        $marcado='#DCDCDC';
                                    }else{
                                        $marcado = '' ;
                                    }
                            ?>
                                    <table width="100%">
                                        <tr style="width:100%; background-color:<?=$marcado;?>;">
                                            <td style="width:98%; text-align: left;">
                                                - <a href="javascript: listaPessoalFiltro('filtro_fta_ftaid', <?= $tp['ftaid']; ?>);"><?= $tp['ftadescricao']; ?></a>
                                            </td>
                                            <td style="width:2%; text-align: right; font-style:oblique; font-weight: bold;">
                                                <?PHP
                                                    $sqlb = "
                                                        SELECT count(x.contagem) FROM
                                                        ( SELECT distinct fdp.fdpcpf as contagem
                                                              FROM
                                                              gestaopessoa.ftdadopessoal as fdp
                                                              LEFT JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
                                                              LEFT JOIN gestaopessoa.ftdadofuncional ful ON ful.fdpcpf = fdp.fdpcpf
                                                              LEFT  JOIN gestaopessoa.ftatividadedesenvolvida as atividadedesenv ON fdp.fdpcpf = atividadedesenv.fdpcpf
                                                              WHERE fdp.fdpcpf IS NOT NULL
                                                              AND fst.fstid <> 4
                                                              " . ($filtroGestor ? " AND fst.fstid in (" . $filtroGestor . ") " : '' ) . "
                                                              " . ($filtroAidp ? " AND ful.fulid in (" . $filtroAidp . ") " : '' ) . "
                                                              AND atividadedesenv.ftaid = " . $tp['ftaid'] . "
                                                        ) AS x ";
                                                    echo $db->pegaUm($sqlb);
                                                ?>
                                            </td>
                                        </tr>
                                    </table>
                            <? } ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <img id="img_atividade_nivel" src="../imagens/mais.gif" onclick="onOff('atividade_nivel');">&nbsp;<b>Atividade Desenvolvida X Nível</b>
                        </td>
                    </tr>
                    <tr id="atividade_nivel" style="display:none;">
                        <td colspan="2">
                            <?php
                                $sql = "SELECT * FROM gestaopessoa.fttiponivelatividadedesenvolvid";
                                $rs = $db->carregar($sql);
                                $rs = $rs? $rs: array();
                                foreach($rs as $i => $tp){
                                    if(($i%2) == 0){
                                        $marcado='#DCDCDC';
                                    }else{
                                        $marcado = '' ;
                                    }
                            ?>
                                    <table width="100%">
                                        <tr style="width:100%; background-color:<?=$marcado;?>;">
                                            <td style="width:98%; text-align: left;">
                                                - <a href="javascript: listaPessoalFiltro('filtro_fta_fnaid', <?= $tp['fnaid']; ?>);"><?= $tp['fnadescricao']; ?></a>
                                            </td>
                                            <td style="width:2%; text-align: right; font-style:oblique; font-weight: bold;">
                                                <?php
                                                    $sqlb = "
                                                        SELECT count(x.contagem) FROM
                                                        ( SELECT distinct fdp.fdpcpf as contagem
                                                            FROM
                                                            gestaopessoa.ftdadopessoal as fdp
                                                            LEFT JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
                                                            LEFT JOIN gestaopessoa.ftdadofuncional ful ON ful.fdpcpf = fdp.fdpcpf
                                                            LEFT JOIN gestaopessoa.ftatividadedesenvolvida as atividadedesenv ON fdp.fdpcpf = atividadedesenv.fdpcpf
                                                            WHERE fdp.fdpcpf IS NOT NULL
                                                            AND fst.fstid <> 4
                                                            " . ($filtroGestor ? " AND fst.fstid in (" . $filtroGestor . ") " : '' ) . "
                                                            " . ($filtroAidp ? " AND ful.fulid in (" . $filtroAidp . ") " : '' ) . "
                                                            AND atividadedesenv.fnaid = " . $tp['fnaid'] . "
                                                        ) AS x ";
                                                echo $db->pegaUm($sqlb);
                                                ?>
                                            </td>
                                        </tr>
                                    </table>
                            <? } ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <BR><b>Experiência Anterior</b>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <img id="img_experiencia_tipo" src="../imagens/mais.gif" onclick="onOff('experiencia_tipo');">&nbsp;<b>Experiência Anterior X Tipo</b>
                        </td>
                    </tr>
                    <tr id="experiencia_tipo" style="display:none;">
                        <td colspan="2">
                                <?php
                                    $sql = "SELECT * FROM gestaopessoa.fttipoexperienciaanterior";
                                    $rs = $db->carregar($sql);
                                    $rs = $rs? $rs: array();
                                    foreach($rs as $i => $tp){
                                        if(($i%2) == 0){
                                            $marcado='#DCDCDC';
                                        }else{
                                            $marcado = '' ;
                                        }
                                ?>
                                        <table width="100%">
                                            <tr style="width:100%; background-color:<?=$marcado;?>;">
                                                <td style="width:98%; text-align: left;">
                                                    - <a href="javascript: listaPessoalFiltro('filtro_fte_fteid', <?= $tp['fteid']; ?>);"><?= $tp['ftedescricao']; ?></a>
                                                </td>
                                                <td style="width:2%; text-align: right; font-style:oblique; font-weight: bold;">
                                                    <?php
                                                        $sqlb = "
                                                            SELECT count(x.contagem) FROM
                                                            ( SELECT distinct fdp.fdpcpf as contagem
                                                                    FROM
                                                                    gestaopessoa.ftdadopessoal as fdp
                                                                    LEFT JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
                                                                    LEFT JOIN gestaopessoa.ftdadofuncional ful ON ful.fdpcpf = fdp.fdpcpf
                                                                    LEFT JOIN gestaopessoa.ftexperienciaanterior AS fte ON fdp.fdpcpf = fte.fdpcpf
                                                                    WHERE fdp.fdpcpf IS NOT NULL
                                                                    AND fst.fstid <> 4
                                                                    " . ($filtroGestor ? " AND fst.fstid in (" . $filtroGestor . ") " : '' ) . "
                                                                    " . ($filtroAidp ? " AND ful.fulid in (" . $filtroAidp . ") " : '' ) . "
                                                                    AND fte.fteid = " . $tp['fteid'] . "
                                                            ) AS x
                                                        ";
                                                        echo $db->pegaUm($sqlb);
                                                    ?>
                                                </td>
                                            </tr>
                                        </table>
                                <? } ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <img id="img_experiencia_nivel" src="../imagens/mais.gif" onclick="onOff('experiencia_nivel');">&nbsp;<b>Experiência Anterior X Nível</b>
                        </td>
                    </tr>
                    <tr id="experiencia_nivel" style="display: none;">
                        <td colspan="2">
                                <?php
                                    $sql = "SELECT * FROM gestaopessoa.fttiponivelexperienciaanterior";
                                    $rs = $db->carregar($sql);
                                    $rs = $rs? $rs: array();
                                    foreach($rs as $i => $tp){
                                        if(($i%2) == 0){
                                            $marcado='#DCDCDC';
                                        }else{
                                            $marcado = '' ;
                                        }
                                ?>
                                        <table width="100%">
                                            <tr style="width:100%; background-color:<?=$marcado;?>;">
                                                <td style="width:98%; text-align: left;">
                                                    - <a href="javascript: listaPessoalFiltro('filtro_fte_fneid', <?= $tp['fneid']; ?>);"><?= $tp['fnedescricao']; ?></a>
                                                </td>
                                                <td style="width:2%; text-align: right; font-style:oblique; font-weight: bold;">
                                                    <?php
                                                        $sqlb = "
                                                            SELECT count(x.contagem) FROM
                                                            ( SELECT distinct fdp.fdpcpf as contagem
                                                                  FROM
                                                                  gestaopessoa.ftdadopessoal as fdp
                                                                  LEFT JOIN gestaopessoa.ftsituacaotrabalhador AS fst ON fst.fstid = fdp.fstid
                                                                  LEFT JOIN gestaopessoa.ftdadofuncional ful ON ful.fdpcpf = fdp.fdpcpf
                                                                  LEFT JOIN gestaopessoa.ftexperienciaanterior AS fte ON fdp.fdpcpf = fte.fdpcpf
                                                                  WHERE fdp.fdpcpf IS NOT NULL
                                                                  AND fst.fstid <> 4
                                                                  " . ($filtroGestor ? " AND fst.fstid in (" . $filtroGestor . ") " : '' ) . "
                                                                  " . ($filtroAidp ? " AND ful.fulid in (" . $filtroAidp . ") " : '' ) . "
                                                                  AND fte.fneid = " . $tp['fneid'] . "
                                                        ) AS x ";
                                                        echo $db->pegaUm($sqlb);
                                                    ?>
                                                </td>
                                            </tr>
                                        </table>
                                <? } ?>
                        </td>
                    </tr>
                </table>
        <?php } ?>
            </td>
    </tr>
</table>

<script>
    function listaPessoalFiltro(filtro, valor) {
        window.location.href = 'gestaopessoa.php?modulo=principal/forca_trabalho/lista_grid_servidores&acao=A&' + filtro + '=' + valor;
    }
    function listaPessoal(filtro, id) {
        var filtro = document.getElementById(filtro);
        filtro.value = id;
        document.formulario[0].submit();
    }
    function listaPessoalMedia(filtro, id) {
        var filtro = document.getElementById(filtro);
        filtro.value = 't';
        document.formulario[0].submit();

    }
    function listaPessoalCompleta() {
        window.location.href = 'gestaopessoa.php?modulo=principal/listaPessoalAvaliacao&acao=A';
    }
    function onOff(div) {
        if (!div) {
            return false;
        }
        if (document.getElementById(div).style.display == 'none') {
            document.getElementById(div).style.display = '';
            document.getElementById('img_' + div).src = '../imagens/menos.gif';
        } else {
            document.getElementById(div).style.display = 'none';
            document.getElementById('img_' + div).src = '../imagens/mais.gif';
        }
    }
</script>