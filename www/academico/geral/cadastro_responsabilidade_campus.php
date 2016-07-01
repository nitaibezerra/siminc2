<?PHP
    include "config.inc";
    include "../_funcoes.php";
    header('Content-Type: text/html; charset=iso-8859-1');

    include APPRAIZ . "includes/classes_simec.inc";
    include APPRAIZ . "includes/funcoes.inc";
    include_once "../_constantes.php";

    $db = new cls_banco();

    $usucpf = $_REQUEST['usucpf'];
    $pflcod = $_REQUEST['pflcod'];

    #BUSCA PERFIL DO USUÁRIO
    $perfil = pegaPerfilGeral( $usucpf );

    if( in_array(PERFIL_DIRETOR_CAMPUS, $perfil) ){
        $sql = "
            SELECT e.entid
            FROM academico.usuarioresponsabilidade AS u

            JOIN entidade.entidade AS e ON e.entid = u.entid
            JOIN entidade.funcaoentidade AS f ON f.entid = e.entid

            WHERE  usucpf = '{$usucpf}' AND funid IN (".ACA_ID_UNIVERSIDADE.",".ACA_ID_ESCOLAS_TECNICAS.") AND rpustatus = 'A'
        ";
        $result = $db->carregarColuna($sql);

        if( $result[0] > 0 ){
            $campos_relacionados_unid = " AND ea.entid  IN ( ". implode($result, ',') ." )";
        }else{
            $db->sucesso( '','', 'Não é possível realizar esse operação, primeiramente deve ser atribuido uma "Unidade"!', 'S');
        }
    }

    #INICIO REGISTRO RESPONSABILIDADES
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    if (is_array($_POST['edtSelecionadas'])) {
        $sql = "UPDATE academico.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '{$usucpf}' AND pflcod = {$pflcod} AND entid in (SELECT entid FROM entidade.funcaoentidade WHERE funid in (17,18))";
        $db->executar($sql);

        if ($_POST['edtSelecionadas'][0]) {
            foreach ($_POST['edtSelecionadas'] as $entid) {

                $sql = "
                    INSERT INTO academico.usuarioresponsabilidade(
                        pflcod, usucpf, rpustatus, rpudata_inc, entid
                    ) VALUES (
                        {$pflcod}, '{$usucpf}', 'A', 'now()', {$entid}
                    )
                ";
                $db->executar($sql);
            }
        }
        $db->commit();
        $db->sucesso('', '&acao=A&pflcod='.$pflcod.'&usucpf='.$usucpf, 'Operação realizada com sucesso', 'S', 'S', 'S');
    }
    #FIM REGISTRO RESPONSABILIDADES
?>

<html>
    <head>
        <META http-equiv="Pragma" content="no-cache">
        <title>Unidades</title>

        <script type="text/javascript" src="../../includes/funcoes.js"></script>
        <script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>

        <link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
        <link rel='stylesheet' type='text/css' href='/includes/listagem.css'>

        <style type="text/css">
            input, select, td{
                font-size: 11px !important;
            }
        </style>
    </head>

    <body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" margineight="0" bgcolor="#ffffff">
        <div align=center id="aguarde" style="visibility:hidden; display:none;">
            <img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle" > <font color=blue size="2">Aguarde! Carregando Dados...</font>
        </div>

        <div style="overflow:auto; width:100%; height:auto; border:2px solid #ececec; background-color: White; border: 1px solid black;">
            <form action="" method="POST" id="formulario" name="formulario">
                <input type="hidden" id="requisicao" name="requisicao" value=""/>
                <input type="hidden" id="usucpf" name="usucpf" value="<?=$usucpf;?>">
                <input type="hidden" id="pflcod" name="pflcod" value="<?=$pflcod;?>">

                <div style="position:fixed; background-color: #FFFFFF; top:2%; height:9px; width:99%;">
                    <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="tabela">
                        <tr>
                            <td class ="SubTituloDireita">Tipo de Ensino:</td>
                            <td>
                                <?PHP
                                    $sql = "
                                            SELECT  CASE WHEN o.orgid = 1
                                                        THEN 18
                                                        ELSE 17
                                                    END AS codigo,
                                                    o.orgdesc AS descricao
                                            FROM academico.orgao as o
                                            WHERE o.orgstatus = 'A' AND o.orgid <> 3
                                    ";
                                    $db->monta_combo("orgid", $sql, 'S', 'Selecione...', '', '', '', '310', 'N', 'orgid', false, $orgid, null);
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class ="SubTituloDireita" width="30%">Unidade - Descrição:</td>
                            <td>
                                <?= campo_texto('entnome', 'N', 'S', '', 55, 100, '', '', '', '', 0, 'id="entnome"', '', $entnome, null, '', null); ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloCentro" colspan="2">
                                <input type="button" name="pesquisar" value="Pesquisar" onclick="document.formulario.submit();"/>
                            </td>
                        </tr>
                    </table>
                </div>

                <div style="background-color: #FFFFFF; position:fixed; top:23%; width:100%; height:44%; overflow:auto;">
                    <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="tabela listagem">
                        <tr>
                            <td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4">
                                <strong>Selecione a(s) Unidade(s)</strong>
                            </td>
                        </tr>
                        <tr>
                            <td>&nbsp;</td>
                            <td><b>Código UND.</b></td>
                            <td><b>Unidade</b></td>
                        </tr>

                        <?PHP
                            $reitor = verificaPerfil( PERFIL_REITOR );
                            $pro_reitor = verificaPerfil( PERFIL_PROREITOR );
                            $interlocutor = verificaPerfil( PERFIL_INTERLOCUTOR_INSTITUTO );
                            $interloc_cp = verificaPerfil( PERFIL_INTERLOCUTOR_CAMPUS );

                            if ($_REQUEST['orgid']) {
                                $orgid = $_REQUEST['orgid'];
                                $where[] = " ef.funid = '{$orgid}'";
                            }
                            if ($_REQUEST['entnome']) {
                                $entnome = $_REQUEST['entnome'];
                                $where[] = " public.removeacento(e.entnome) ILIKE public.removeacento( ('%{$entnome}%') ) ";
                            }

                            #CARREGA AS ENTIDADES DO PERFIL REITOR
                            if ($db->testa_superuser() || $reitor || $pro_reitor || $interlocutor || $interloc_cp ) {
                                $sql = "SELECT DISTINCT entid FROM academico.usuarioresponsabilidade WHERE rpustatus = 'A' and pflcod IN ( ".PERFIL_REITOR.", ".PERFIL_PROREITOR.", ".PERFIL_INTERLOCUTOR_INSTITUTO.", ".PERFIL_INTERLOCUTOR_CAMPUS." ) AND usucpf = '" . $usucpf . "'";
                                $arrEntidReitor2 = $db->carregarColuna($sql);
                                if ($arrEntidReitor2[0]) {
                                    $arrEntidReitor = implode(",", $arrEntidReitor2);
                                    $andEntidReitor = " and ea.entid in (" . $arrEntidReitor . ") ";
                                }
                            }

                            if($where != ""){
                                $WHERE = "WHERE e.entstatus = 'A' AND ef.funid IN (17, 18) AND ".implode(' AND ', $where);
                            }else{
                                $WHERE = "WHERE e.entstatus = 'A' AND ef.funid IN (17, 18)";
                            }

                            $cabecalho = 'Selecione a(s) Unidade(s)';

                            $sql = "
                                SELECT  e.entid AS codigo,
                                        upper(e.entnome) as descricao

                                FROM entidade.entidade e2

                                INNER JOIN entidade.entidade e ON e2.entid = e.entid
                                INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
                                INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
                                LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
                                LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod

                                {$WHERE}
                                    
                                {$andEntidReitor}
                                    
                                {$campos_relacionados_unid}

                                ORDER BY e.entnome
                            ";
                            $RS = @$db->carregar($sql);

                            if ($RS) {
                                $nlinhas = count($RS) - 1;

                                for ($i = 0; $i <= $nlinhas; $i++) {
                                    extract($RS[$i]);

                                    if (fmod($i, 2) == 0){
                                        $cor = '#f4f4f4';
                                    }else{
                                        $cor = '#e0e0e0';
                                    }
                        ?>
                                    <tr bgcolor="<?= $cor ?>">
                                        <td align="center">
                                            <input type="checkbox" name="codigo" id="<?= $codigo ?>" value="<?= $codigo ?>" onclick="retorna(<?= $i ?>);">
                                            <input type="Hidden" name="descricao_select" value="<?= $codigo . ' - ' . $descricao ?>"></td>
                                        <td align="right" style="color:blue; width:85px; text-align:center;"> <?= $codigo ?> </td>
                                        <td><?= $descricao?></td>
                                    </tr>

                        <?PHP
                                }
                            } else {
                        ?>
                                <tr>
                                    <td align="center" colspan="3" style="color: rgb(204, 0, 0);">Não foram encontrados Registros.</td>
                                </tr>
                                <?php
                            }
                        ?>
                    </table>
                </div>
            </form>

            <div style="background-color: #FFFFFF; position:fixed; top:69%; width:100%; height:25%;">
                <form name="formassocia" method="POST">
                    <input type="hidden" name="usucpf" value="<?= $usucpf ?>">
                    <input type="hidden" name="pflcod" value="<?= $pflcod ?>">
                    <select multiple size="8" name="edtSelecionadas[]" id="edtSelecionadas" style="width:99%;" class="CampoEstilo" onkeydown="javascript:combo_popup_remove_selecionados( event, 'edtSelecionadas' );">
                        <?PHP
                            $sql = "
                                SELECT  DISTINCT e.entid as codigo,
                                        initcap(e.entnome) as descricao
                                FROM entidade.entidade e

                                INNER JOIN entidade.funcaoentidade fe ON fe.entid = e.entid AND fe.funid IN (17, 18)
                                INNER JOIN academico.usuarioresponsabilidade ur ON ur.entid = e.entid AND ur.rpustatus = 'A'

                                WHERE ur.rpustatus='A' AND ur.usucpf = '{$usucpf}' AND ur.pflcod = {$pflcod}

                                ORDER BY descricao
                            ";
                            $RS = @$db->carregar($sql);

                            if (is_array($RS)) {
                                $nlinhas = count($RS) - 1;
                                if ($nlinhas >= 0) {
                                    for ($i = 0; $i <= $nlinhas; $i++) {
                                        foreach ($RS[$i] as $k => $v)
                                            ${$k} = $v;
                                        print " <option value=\"$codigo\">$codigo - $descricao</option>";
                                    }
                                }
                            } else {
                        ?>
                            <option value="">Clique na Unidade.</option>
                        <?PHP
                            }
                        ?>
                    </select>
                </form>
            </div>

            <div style="background-color: #FFFFFF; position:fixed; top:94%; width:100%; height:25%;">
                <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
                    <tr bgcolor="#c0c0c0">
                        <td align="right" style="padding:3px;" colspan="3">
                            <input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect); document.formassocia.submit();" id="ok">
                        </td>
                    </tr>
                </table>
            </div>
        </div>

<script language="JavaScript">

    var campoSelect = document.getElementById("edtSelecionadas");

    if (campoSelect.options[0].value != ''){
        for(var i=0; i<campoSelect.options.length; i++){
            document.getElementById(campoSelect.options[i].value).checked = true;
        }
    }

    function abreconteudo(objeto){
        if (document.getElementById('img' + objeto).name == '+'){
            document.getElementById('img' + objeto).name = '-';
            document.getElementById('img' + objeto).src = document.getElementById('img' + objeto).src.replace('mais.gif', 'menos.gif');
            document.getElementById(objeto).style.visibility = "visible";
            document.getElementById(objeto).style.display = "";
        }else{
            document.getElementById('img' + objeto).name = '+';
            document.getElementById('img' + objeto).src = document.getElementById('img' + objeto).src.replace('menos.gif', 'mais.gif');
            document.getElementById(objeto).style.visibility = "hidden";
            document.getElementById(objeto).style.display = "none";
        }
    }

    function retorna(objeto){
        tamanho = campoSelect.options.length;
        if (campoSelect.options[0].value == '') {
            tamanho--;
        }

        var codigo = document.getElementsByName('codigo');
        var descricao = document.getElementsByName('descricao_select');

        if (codigo[objeto].checked == true) {
            campoSelect.options[tamanho] = new Option(descricao[objeto].value, codigo[objeto].value, false, false);
            sortSelect(campoSelect);
        } else {
            for (var i = 0; i <= campoSelect.length - 1; i++) {
                if (codigo[objeto].value == campoSelect.options[i].value){
                    campoSelect.options[i] = null;
                }
            }

            if (!campoSelect.options[0]){
                campoSelect.options[0] = new Option('Clique no Municípo.', '', false, false);
            }
            sortSelect(campoSelect);
        }
    }

    function pesquisar(param){
        if(trim(param) == 'fil'){
            $('#formulario').submit();
        }else{
            $('#formulario').submit();
        }
    }
</script>
