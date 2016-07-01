<?php
    include "config.inc";
    header('Content-Type: text/html; charset=iso-8859-1');
    include APPRAIZ . "includes/classes_simec.inc";
    include APPRAIZ . "includes/funcoes.inc";

    $db     = new cls_banco();
    $usucpf = $_REQUEST['usucpf'];
    $pflcod = $_REQUEST['pflcod'];
    $acao   = $_REQUEST["acao"];
    $orgid  = $_REQUEST['orgid'];
    $gravar = $_REQUEST['gravar'];
    $unicod = $_REQUEST["uniresp"];
    
    if ($orgid == 1) {
        $funid = '12';
    } elseif ($orgid == 2) {
        $funid = '11,14';
    } elseif ($orgid == 3) {
        $funid = '102';
    }

    if ($_POST && $gravar == 1) {
        atribuiUnidade($usucpf, $pflcod, $unicod);
    }


    function recuperaOrgao($orgid = null) {
        global $db;

        $sql = "
            SELECT  orgdesc 
            FROM gestaopessoa.orgao oo
            WHERE orgstatus = 'A'
        ";
        return $dsc = $db->pegaUm($sql);
    }

    #Função que lista as unidades
    function listaUnidades() {
        global $db, $funid, $orgid;
        
        $where  = array();
        $campo  = array();
        $from   = array();

        if ($db->testa_superuser() && !$orgid) {
            echo "<tr><td style='color:red;'>Faça sua busca...</td></tr>";
            return;
        }

        #SQL para buscar unidades existentes
        $sql = "
            SELECT  DISTINCT e.entid,
                    ".implode("", $campo)."
                    e.entnome
            FROM entidade.entidade e
            ".implode("", $from)."
            INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid
            WHERE entstatus='A' AND fen.funid IN ('" . str_replace(",", "','", $funid) . "')
            " . ($where ? " AND " . implode(" AND ", $where) : '') . "
            ORDER BY  entnome
        ";
        $unidadesExistentes = $db->carregar($sql);
            
        if (!$unidadesExistentes) {
            echo "<tr><td style='color:red;'>Busca não retornou registros...</td></tr>";
            return;
        }

        $count = count($unidadesExistentes);

        $orgdsc = recuperaOrgao( $orgid );
        #Monta as TR e TD com as unidades
        for ($i = 0; $i < $count; $i++) {
            $codigo     = $unidadesExistentes[$i]["entid"];
            $descricao  = $unidadesExistentes[$i]["entnome"];

            if (fmod($i, 2) == 0) {
                $cor = '#f4f4f4';
            } else {
                $cor = '#e0e0e0';
            }

            echo "
                <tr bgcolor=\"" . $cor . "\">
                    <td align=\"right\" width=\"10%\">
                        <input type=\"Checkbox\" name=\"unicod\" id=\"" . $codigo . "\" value=\"$orgid|$codigo\" onclick=\"retorna('" . $i . "');\">
                        <!--<input type=\"hidden\" name=\"unidsc\" value=\"" . ($funid == 1 ? $orgdsc . " - " . $descricao : $descricao . " - " . $orgdsc) . "\">-->
                        <input type=\"hidden\" name=\"unidsc\" value=\"" . $orgdsc . " - " . $descricao . "\">
                    </td>
                    <td>
                        ".$descricao."
                    </td>
                </tr>
            ";
        }
    }

    #Função que atribui a responsabilidade de uma unidade ao usuário
    function atribuiUnidade($usucpf, $pflcod, $entid) {
        global $db;

        $data = date("Y-m-d H:i:s");

        $sql = "
            UPDATE gestaopessoa.usuarioresponsabilidade 
                SET rpustatus = 'I' 
            WHERE usucpf = '{$usucpf}' AND pflcod = '{$pflcod}' AND entid IS NOT NULL
        ";
        $db->executar($sql);

        if( is_array( $entid ) && !empty( $entid[0] ) ) {
            $count = count( $entid );

            #Insere a nova unidade
            $sql_insert = " INSERT INTO gestaopessoa.usuarioresponsabilidade (pflcod, usucpf, rpustatus, rpudata_inc, entid) VALUES ";

            for ($i = 0; $i < $count; $i++) {
                list(, $entidade) = explode("|", $entid[$i]);

                $arrSql[] = " ( '{$pflcod}', '{$usucpf}',  'A',  '{$data}', '{$entidade}' ) ";
            }

            $sql_insert = (string) $sql_insert . implode(",", $arrSql);

            $db->executar($sql_insert);
        }
        $db->commit();
        
        die("
            <script>
                alert('Operação realizada com sucesso!');
                window.parent.opener.location.href = window.opener.location;
                //self.close();
            </script>"
        );
    }

    function buscaUnidadesCadastradas($usucpf, $pflcod) {
        global $db, $unicod;

        if (!$_POST['gravar'] && $_REQUEST["uniresp"][0]) {
            foreach ($_REQUEST["uniresp"] as $v) {
                list(, $entid[]) = explode('|', $v);
            }
            $where = " e.entid IN (" . implode(',', $entid) . ")";
        } else {
            $where = " (ur.usucpf = '{$usucpf}' AND ur.pflcod = {$pflcod})";
        }

        $sql = "
            SELECT  DISTINCT e.entid as codigo, 
                    e.entnome as descricao,
                    fen.funid 
            FROM entidade.entidade e
            JOIN entidade.funcaoentidade fen ON fen.entid = e.entid AND fen.funid IN (11,12,14,102)
            INNER JOIN gestaopessoa.usuarioresponsabilidade ur ON ur.entid = fen.entid AND ur.rpustatus = 'A'
            WHERE ".$where;

        $RS = @$db->carregar($sql);

        if (is_array($RS)) {
            $nlinhas = count($RS) - 1;
            if ($nlinhas >= 0) {

                for ($i = 0; $i <= $nlinhas; $i++) {

                    foreach ($RS[$i] as $k => $v) {
                        ${$k} = $v;
                    }

                    if ($funid == 12) {
                        $orgid = 1;
                    } elseif ($funid == 11 || $funid == 14) {
                        $orgid = 2;
                    } else {
                        $orgid = 3;
                    }

                    $orgdsc[$funid] = $orgdsc[$funid] ? $orgdsc[$funid] : recuperaOrgao($orgid);

                    print " <option value=\"$orgid|$codigo\">{$orgdsc[$funid]} - $descricao</option>";
                }
            }
        } else {
            print '<option value="">Clique faça o filtro para selecionar.</option>';
        }
    }
?>

    <html>
        <head>
            <meta http-equiv="Pragma" content="no-cache">
            <title>Unidades</title>
            <script language="JavaScript" src="../../includes/funcoes.js"></script>
            <link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
            <link rel='stylesheet' type='text/css' href='../../includes/listagem.css'>
        </head>
        
        <body leftmargin="0" topmargin="5" bottommargin="5" marginwidth="0" marginheight="0" bgcolor="#ffffff">
            <div align=center id="aguarde">
                <img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle">
                <font color=blue size="2">Aguarde! Carregando Dados...</font>
            </div>
            
            <? flush(); ?>
            
            <form name="formulario" action="<?= $_SERVER['REQUEST_URI'] ?>" method="post">
               <!-- <table style="width:100%; display:none;" id="filtro" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"> -->
                <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
                    <tr>
                        <td class="subtituloCentro" colspan="2" >&nbsp;</td>
                    </tr>							
                    <tr>
                        <td class="subtitulodireita" width="30%">Tipo de Ensino:</td>
                        <td>
                            <?php
                                $sql = "
                                    SELECT  oo.orgid AS codigo, 
                                            orgdesc AS descricao
                                    FROM gestaopessoa.orgao oo
                                    WHERE orgstatus = 'A'
                                ";
                                $db->monta_combo('orgid', $sql, 'S', 'Selecione...', 'filtroFunid', '', '', '290', 'N', 'orgid', false, $orgid, null);
                                echo '&nbsp;<img src="/imagens/obrig.gif" title="Indica campo obrigatório">';
                            ?>
                        </td>
                    </tr>							
                </table>
                
                <!-- Lista de Unidades -->
                
                <div id="tabela" style="overflow:auto; width:100%; height:330px; border:2px solid #ececec; background-color: #ffffff;">	
                    <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
                        <script language="JavaScript">
                            document.getElementById('tabela').style.display = "none";
                        </script>
                        <thead>
                            <tr>
                                <td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="3">
                                    <strong>Selecione a(s) Unidade(s)</strong>
                                </td>
                            </tr>
                        </thead>
                        
                        <?php listaUnidades(); ?>
                        
                    </table>
                </div>
                
                <script language="JavaScript">
                    document.getElementById('filtro').style.display = 'block';
                </script>
                
                <!-- Unidades Selecionadas -->
                <input type="hidden" name="usucpf" value="<?= $usucpf ?>">
                <input type="hidden" name="pflcod" value="<?= $pflcod ?>">
                <select multiple size="8" name="uniresp[]" id="uniresp" style="width:100%;" onkeydown="javascript:combo_popup_remove_selecionados(event, 'uniresp');" class="CampoEstilo" onchange="//moveto(this);">				
                    <?php
                        buscaUnidadesCadastradas($usucpf, $pflcod);
                    ?>
                </select>
                
                <!-- Submit do Formulário -->
                <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
                    <tr bgcolor="#c0c0c0">
                        <td align="right" style="padding:3px;" colspan="3">
                            <input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);
                                document.getElementsByName('gravar')[0].value = 1;
                                document.formulario.submit();" id="ok">
                            <input type="hidden" name="gravar" value="">
                        </td>
                    </tr>
                </table>
            </form>
            
    <script language="JavaScript" type="text/JavaScript">
        document.getElementById('aguarde').style.visibility = "hidden";
        document.getElementById('aguarde').style.display  = "none";
        document.getElementById('tabela').style.display  = 'block';

        var campoSelect = document.getElementById("uniresp");

        <?php
            if ($funid){
        ?>
                if (campoSelect.options[0].value != ''){
                    for(var i=0; i<campoSelect.options.length; i++){
                        var id = campoSelect.options[i].value.split('|');

                        if (document.getElementById(id[1])){
                            document.getElementById(id[1]).checked = true;
                        }
                    }
                }
        <? } ?>


        function abreconteudo(objeto){
            if (document.getElementById('img'+objeto).name=='+'){
                document.getElementById('img'+objeto).name='-';
                document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('mais.gif', 'menos.gif');
                document.getElementById(objeto).style.visibility = "visible";
                document.getElementById(objeto).style.display  = "";
            }else{
                document.getElementById('img'+objeto).name='+';
                document.getElementById('img'+objeto).src = document.getElementById('img'+objeto).src.replace('menos.gif', 'mais.gif');
                document.getElementById(objeto).style.visibility = "hidden";
                document.getElementById(objeto).style.display  = "none";
            }
        }

        function retorna(objeto){
            tamanho = campoSelect.options.length;

            if (campoSelect.options[0].value == ''){
                tamanho--;
            }
            if (document.formulario.unicod[objeto].checked == true){
                campoSelect.options[tamanho] = new Option(document.formulario.unidsc[objeto].value, document.formulario.unicod[objeto].value, false, false);
                sortSelect(campoSelect);
            }else{
                for(var i=0; i<=campoSelect.length-1; i++){
                    if (document.formulario.unicod[objeto].value == campoSelect.options[i].value){
                        campoSelect.options[i] = null;
                    }
                }
                if (!campoSelect.options[0]){
                    campoSelect.options[0] = new Option('Clique na Unidade.', '', false, false);
                }
                sortSelect(campoSelect);
            }
        }

        function moveto(obj){
            if (obj.options[0].value != '') {
                if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
                    abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
                }
                document.getElementById(obj.value).focus();
            }
        }

        function filtroFunid (id) {
            var d       = document;
            var orgid   = d.getElementsByName('orgid')[0]  ? d.getElementsByName('orgid')[0].value : '';
            var estuf   = d.getElementsByName('estuf')[0]  ? d.getElementsByName('estuf')[0].value : '';;
            var muncod  = d.getElementsByName('muncod')[0] ? d.getElementsByName('muncod')[0].value : '';

            if (!orgid){
                alert('Selecione um "tipo de ensino" afim de efetuar o filtro!');
                return false;
            }
            selectAllOptions(campoSelect);
            d.formulario.submit();
            //window.location.href = '?pflcod=<?= $_GET['pflcod']; ?>&usucpf=<?= $_GET['usucpf']; ?>&funid='+funid+'&estuf='+estuf+'&muncod='+muncod;
        }

    </script>
    
        </body>
    </html>
