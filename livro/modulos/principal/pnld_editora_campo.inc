<?php

include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';


$responsabilidade = retornaResponsabilidades($_SESSION['usucpf'], 'C', 'cod');

$arrPerfil = arrayPerfil();


if ($_REQUEST['op'] == 'download' && $_REQUEST['arqid']) {
    $param = array();
    $param["arqid"] = $_REQUEST['arqid'];
    DownloadArquivo($param);
    exit;
}


if ($_REQUEST['op'] == 'delete' && $_REQUEST['arqid']) {
    $param = array();
    $param["arqid"] = $_REQUEST['arqid'];
    $param["tipo"] = $_REQUEST['tipo'];
    $ok = deletarAnexo($param);
    unset($_REQUEST['arqid']);
    unset($_REQUEST['op']);
    
    die("
        <script>
            alert('Operação realizada com sucesso!');
            location.href='livro.php?modulo=principal/pnld_editora_campo&acao=A';
	</script>
    ");
}

//formulario para anexo
if ($_REQUEST['cobid']) {

    //anexa arquivo
    if ($_POST['submetido']) {
        if (isset($_POST['cobid'])) {
            if ($_FILES['arquivo']['size']) {
                $dados["cobid"] = $_REQUEST["cobid"];
                $dados["crsid"] = $_REQUEST["crsid"];
                $dados["tipo"] = $_REQUEST["tipo"];
                $dados["arqdescricao"] = $_REQUEST["parecerdsc"];

                //insere arquivo
                $arqid = EnviarArquivo($_FILES['arquivo'], $dados);

                if(!$arqid){
                    die("
                        <script>
                            alert(\"Problemas no envio do arquivo.\");
                            history.go(-1);
                        </script>
                    ");
                }

                //atualiza o registro na tabela livro.pnaiclivro
                if($dados["tipo"]=='1'){
                    //$sql = "UPDATE livro.campoparecer SET arqid=$arqid, crsid=".$dados["crsid"].", cnpdsc=".$dados["arqdescricao"]." WHERE cobid=".$dados["cobid"];
                    $sql = "INSERT INTO livro.campoparecer(cobid, cnpdsc, usucpfmec, arqid, cnpstatus, crsid)
                            VALUES (".$dados["cobid"].",
                                    '".$dados["arqdescricao"]."',
                                    '".$_SESSION['usucpf']."',
                                    ".$arqid.",
                                    'A',
                                    ".$dados["crsid"].")";
                }else{
                    //$sql = "UPDATE livro.pnaiclivro SET arqidrecurso=$arqid WHERE cobid=".$dados["cobid"];
                    $cnpid = $db->pegaUm("select cnpid from livro.campoparecer where cobid = " . $dados['cobid']);
                    $sql = "INSERT INTO livro.camporespostaparecer(cnpid, crpdsc, usucpfobra, arqid, crpstatus)
                            VALUES (".$cnpid.",
                                    '".$dados["arqdescricao"]."',
                                    '".$_SESSION['usucpf']."',
                                    ".$arqid.",
                                    'A')";

                }
                $db->executar($sql);
                $db->commit();


                if ($arqid) {
                    unset($_FILES['arquivo']['size']);
                    die("
                        <script>
                            alert('Operação realizada com sucesso!');
                            window.opener.location.href='livro.php?modulo=principal/pnld_editora_campo&acao=A';
                            self.close();
                        </script>
                    ");
                }
            }
        }
    }
    
?>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/layout/pdeinterativo/css/simec.css"/>
    
    <script type="text/javascript" src="../../includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>

    <script type="text/javascript">
        
        function anexarArquivo(tipo){
            var req  = document.getElementById('req');
            var form = document.getElementById('formA');

            if (document.getElementById('arquivo').value == '') {
                alert('Por favor selecione um arquivo no formato PDF.');
                return false;
            }
            if (form.crsid.value == '' && tipo == 1) {
                alert('Por favor informe a situaçao.');
                return false;
            }
            if (!verificaExtensaoEditora('arquivo')){
                return false;
            }

            form.submit();
        }

        function verificaExtensaoEditora(id) {
            arquivo = document.getElementById(id);
            if (arquivo.value != '') {
                arFile = arquivo.value.split('.');
                indice = arFile.length - 1;
                extensao = arFile[indice].toLowerCase();
                if (extensao != 'pdf') {
                    alert('Extensão não permitida! Envie apenas arquivo com extensão PDF.');
                    arquivo.value = '';
                    return false;
                }
            }
            return true;
        }
        
    </script>

    <form id="formA" method="post" enctype="multipart/form-data" action="">
        <input type="hidden" id="submetido" name="submetido" value="1" />
        <input type="hidden" id="cobid" name="cobid" value="<?= $_REQUEST['cobid'] ?>" />
        <input type="hidden" id="tipo" name="tipo" value="<?= $_REQUEST['tipo'] ?>" />
        <?if($_REQUEST['tipo'] == 2){?>
        	<input type="hidden" id="crsid" name="crsid" value="" />
        <?}?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="3" cellPadding="5" align="center" width="100%">
            <tr>
                <td class="SubtituloTabela" align="center" colspan="2">
                    <?if($_REQUEST['tipo'] == 2){?>
                    	<b>Inserir Arquivo / Recurso EDITORA</b>
                    <?}else{?>
                    	<b>Inserir Arquivo / Parecer MEC</b>
                    <?}?>
                </td>
            </tr>
            <tr>
                <td bgcolor="" align="center" colspan="2">
                    <img border="0" src="/imagens/obrig.gif" /> Indica campo obrigatório
                </td>
            </tr>
            <tr>
                <td align="right" class="SubTituloDireita" style="width:100px;">Título:</td>
                <td>
                    <?PHP
                        if ($_REQUEST['cobid']) {
                            echo $db->pegaUm("select cobcodigo || ' - ' || cobdsc || ' - ' || cobcomposicao  from livro.campoobras where cobid = " . $_REQUEST['cobid']);
                        }
                    ?>
                </td>
            </tr>
            <tr>
                <td align="right" class="SubTituloDireita" style="width:100px;">Arquivo:</td>
                <td>
                    <input type="file" id="arquivo" name="arquivo">
                    <img border="0" src="/imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td align="right" class="SubTituloDireita">
                    <?if($_REQUEST['tipo'] == 2){?>
                        Recurso:
                    <?}else{?>
                        Parecer:
                    <?}?>
                </td>
                <td>
                    <?php echo campo_textarea('parecerdsc', 'S', 'S', '', '10', '2', '', '', 0, '', false, NULL, $parecerdsc, '99%'); ?>
                </td>
            </tr>
            <?if($_REQUEST['tipo'] == 1){?>
            <tr>
                <td align="right" class="SubTituloDireita">Situação:</td>
                <td nowrap="nowrap">
                    <?PHP
                        $sql = "
                            SELECT  crsid AS codigo,
                                    crsdsc AS descricao
                            FROM livro.camporesultado
                            ORDER BY 2
                        ";
                        $db->monta_combo("crsid", $sql, 'S', "-- Informe a Situação --", '', '', '', '', 'S', '', false, $crsid);
                    ?>
                </td>
            </tr>
            <?}?>
            <tr>
                <td class="SubTituloDireita"></td>
                <td align="left">
                    <input type="button" value="Salvar" id="btSalvar" onclick="anexarArquivo('<?=$_REQUEST['tipo']?>')">
                    <input type="button" value="Fechar" onclick="self.close();">
                </td>
            </tr>
        </table>
    </form>
    
<?PHP
    exit;
}

//Chamada de programa lista de obras eja
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

$db->cria_aba($abacod_tela, $url, $parametros);

monta_titulo('PNLD CAMPO', $linha2);

?>

<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />

<link href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>

<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/lib/jquery.cookie.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview-simec.js"></script>

<script type="text/javascript">

    $(document).ready(function(){});

    var janela;

    function onOffCampo(campo) {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none') {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        } else {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }

    function gerarRelatorio(t) {
        var formulario = document.formulario;

        if (t == 1) {
            selectAllOptions(formulario.editora);
            //selectAllOptions(formulario.segmento);
            //selectAllOptions(formulario.colecao);
            //selectAllOptions(formulario.parecer);
        } else {
            formulario.descricao.value = '';
        }
        formulario.submit();
    }

    function anexoParecer(cobid){
    	window.open("livro.php?modulo=principal/pnld_editora_campo&acao=A&tipo=1&cobid="+cobid,"pop","toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,left=600,top=200,width=530,height=320");
    }

    function anexoRecurso(cobid){
    	window.open("livro.php?modulo=principal/pnld_editora_campo&acao=A&tipo=2&cobid="+cobid,"pop","toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,left=600,top=200,width=530,height=280");
    }

    function baixaranexoParecer(arqid){
        location.href="livro.php?modulo=principal/pnld_editora_campo&acao=A&op=download&arqid="+arqid;
    }

    function DelanexoParecer(arqid){
    	if(confirm('Deseja realmente apagar este arquivo?')){
    		location.href="livro.php?modulo=principal/pnld_editora_campo&acao=A&tipo=1&op=delete&arqid="+arqid;
    	}
    }

  	function DelanexoRecurso(arqid){
    	if(confirm('Deseja realmente apagar este arquivo?')){
    		location.href="livro.php?modulo=principal/pnld_editora_campo&acao=A&tipo=2&op=delete&arqid="+arqid;
    	}
    }

    var ns = (navigator.appName.indexOf("Netscape") != -1);
    var d = document;

    function floatDiv(id, sx, sy){

           var el=d.getElementById?d.getElementById(id):d.all?d.all[id]:d.layers[id];
           var px = document.layers ? "" : "px";
           window[id + "_obj"] = el;
           if(d.layers)el.style=el;
           el.cx = el.sx = sx;el.cy = el.sy = sy;
           el.sP=function(x,y){this.style.left=x+px;this.style.top=y+px;};

           el.floatIt=function()
           {
                   var pX, pY;
                   pX = (this.sx >= 0) ? 0 : ns ? innerWidth :
                   document.documentElement && document.documentElement.clientWidth ?
                   document.documentElement.clientWidth : document.body.clientWidth;
                   pY = ns ? pageYOffset : document.documentElement && document.documentElement.scrollTop ?
                   document.documentElement.scrollTop : document.body.scrollTop;
                   if(this.sy<0)
                   pY += ns ? innerHeight : document.documentElement && document.documentElement.clientHeight ?
                   document.documentElement.clientHeight : document.body.clientHeight;
                   this.cx += (pX + this.sx - this.cx)/8;this.cy += (pY + this.sy - this.cy)/8;
                   this.sP(this.cx, this.cy);
                   setTimeout(this.id + "_obj.floatIt()", 40);
           }
           return el;
    }

</script>

<form name="formulario" id="formulario" action="" method="post">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<?PHP
        // Filtros do relatório
        //ver($responsabilidade);
        
        $stSql = "
            SELECT edt.cedid as codigo,
                   edt.cedrazaosocial as descricao
            FROM livro.campoeditora edt";
        
		$stSql .= count($responsabilidade) > 0 ? " WHERE edt.cedid IN (" . implode(", ", $responsabilidade) . ")" : "";
        
        $stSql .= " order by 2";

        $stSqlCarregados = "";

        if (is_array($_POST['editora']) && $_POST['editora'][0]) {
            $stSqlCarregados = "
                SELECT edt.cedid as codigo,
                       edt.cedrazaosocial as descricao
                FROM livro.campoeditora edt
				WHERE  edt.cedid IN (" . implode(", ", $_POST['editora']) . ")";
            $stSqlCarregados .= " order by 2";
        }
        mostrarComboPopup('Editora', 'editora', $stSql, $stSqlCarregados, 'Selecione a Editora');

?>
        <tr>
            <td class="SubTituloDireita" width="10%">Título/Composição/Editora:</td>
            <td>
                <?php
                if ($_POST['descricao'] != "")
                    $descricao = $_POST['descricao'];
                echo campo_texto('descricao', 'N', 'S', '', 50, 60, '', '', '', '', '', 'descricao')
                ?>
            </td>
        </tr>
        <tr>
            <td align="center" colspan="2">
                <input type="button" name="Pesquisar" value="Pesquisar" onclick="javascript:gerarRelatorio(1);"/>
                <input type="button" name="Ver Todos" value="Ver Todos" onclick="location.href='livro.php?modulo=principal/pnld_editora_campo&acao=A';"/>
            </td>
        </tr>
    </table>
</form>


        <?php
        
        extract($_POST);

        if ($editora[0]) {
            $where[0] = " AND edi.cedid " . (( $editora_campo_excludente == null || $editora_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $editora) . "') ";
        }
        if ($descricao) {
            $where[1] = "AND (obr.cobdsc ilike '%" . $descricao . "%' OR obr.cobcomposicao ilike '%" . $descricao . "%' OR edi.cedrazaosocial ilike '%" . $descricao . "%') ";
        }

        $sql = "select  cobid, cobcodigo,
                        cobdsc,
                        cobcomposicao,
                        cedrazaosocial,
                        '' as parecer
                        /*
                        case when arqidparecer is null
                            then '<center> $btnAnexoV </center>'
                            else '<center> <div style=width:83px;> $btnAnexo $btnDe $btnDo </div> </center>'
                        end as anexo,
                        */
                        --resdescricao as parecer,
                        --case when arqidrecurso is null and obr.crsid = 2 then '<center> $btnAnexoRec </center>'
                        	-- when arqidrecurso is not null and obr.crsid = 2 then '<center> <div style=width:83px;> $btnAnexoRec $btnDeRec $btnDoRec </div> </center>'
                        	 --when arqidparecer is not null and obr.crsid = 1 then '<center>-</center>'
                        --end as recurso,

                FROM  livro.campoobras obr
                INNER JOIN livro.campoeditora edi ON edi.cedid = obr.cedid
                WHERE 1=1
                ".$where[0]."
                ".$where[1]."
                " . (checkPerfil(array(PNLD_PERFIL_EDITORA_CAMPO), false) ? (is_array($_SESSION['pnld']['cedid']) ? ' AND edi.cedid in (' . implode(',', $_SESSION['pnld']['cedid']) . ')' : ' and 1=2 ') : '' );

        $sql .= count($responsabilidade) > 0 ? " AND edi.cedid IN (" . implode(", ", $responsabilidade) . ")" : "";
        
        $sql .= " ORDER BY 1,2,3";
        $resultSet = $db->carregar($sql);        
        $resultSet = ($resultSet) ? $resultSet : array();   

?>


<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
    <thead>
        <tr>
            <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor="">
                    <strong>Código</strong>
            </td>
            <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor="">
                <strong>Título</strong>
            </td>
            <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" >
                <strong>Composição</strong>
            </td>
            <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" >
                <strong>Editora</strong>
            </td>
            <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" >
                <strong>Parecer / Recurso da Avaliação Pedagógica</strong></td>
        </tr>
    </thead>
    <tbody>
        <?
        $cor = "#F7F7F7";
        foreach($resultSet as $v){

            if($cor == "#F7F7F7") $cor = "";
            else $cor = "#F7F7F7";
        ?>
            <tr bgcolor="<?=$cor?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$cor?>';">
                <td title="Código"><?=$v['cobcodigo']?></td>
                <td title="Título"><?=$v['cobdsc']?></td>
                <td title="Composição"><?=$v['cobcomposicao']?></td>
                <td title="Editora"><?=$v['cedrazaosocial']?></td>
                <td>

                    <table width="95%" align="center" border="1" cellspacing="0" cellpadding="0" class="tabela">
                        <thead>
                            <tr>
                                <td width="50%" align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor="">
                                    <strong>MEC</strong>
                                </td>
                                <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor="">
                                    <strong>EDITORA / Recurso da Avaliação Pedagógica</strong>
                                </td>
                            </tr>
                        </thead>
                        <tbody>
                            <?
                            $sql = "SELECT cnpid, cnpdsc, usucpfmec, arqid, cnpstatus, crsid
                                    FROM livro.campoparecer
                                    WHERE cobid = ".$v['cobid'];
                            $parecer = $db->carregar($sql);
                            $parecer = ($parecer) ? $parecer : array();
                            $i=0;
                            foreach($parecer as $p){
                                $i++;
                                ?>
                                <tr>
                                    <td align="center">
                                        <table width="90%" align="center" border="0" cellspacing="0" cellpadding="2">
                                            <tr>
                                                <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor="">
                                                    <strong>Arquivo</strong>
                                                </td>
                                                <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor="">
                                                    <strong>Situação</strong>
                                                </td>
                                                <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor="">
                                                    <strong>Parecer</strong>
                                                </td>
                                            </tr>
                                            <tr>
                                                <td align="center">
                                                    <?if( in_array( PNLD_PERFIL_GESTOR_CAMPO, $arrPerfil ) || $db->testa_superuser() ){
                                                        $sqlc = "SELECT count(cnpid) as total FROM livro.camporespostaparecer WHERE cnpid = ".$p['cnpid'];
                                                        $totalResp = $db->pegaUm($sqlc);
                                                        if($totalResp > 0){
                                                            ?>
                                                            <img style='cursor:hand' title='Apagar arquivo' src='../imagens/excluir.gif' onclick="alert('É necesário excluir a resposta primeiro.')">
                                                            &nbsp;
                                                        <?}else{?>
                                                            <img style='cursor:hand' title='Apagar arquivo' src='../imagens/excluir.gif' onclick="DelanexoParecer('<?=$p['arqid']?>')">
                                                            &nbsp;
                                                        <?}?>
                                                    <?}?>
                                                    <img style='cursor:hand' title='Fazer download do arquivo' src='../imagens/clipe.gif' onclick="baixaranexoParecer('<?=$p['arqid']?>')">
                                                </td>
                                                <td align="center">
                                                    <?=$db->pegaUm("select crsdsc from livro.camporesultado where crsid = ".$p['crsid']);?>
                                                </td>
                                                <td align="center">
                                                    <textarea cols="20" rows="2" disabled><?=$p['cnpdsc']?></textarea>
                                                </td>
                                            </tr>
                                        </table>
                                    </td>
                                    <td align="center">
                                        <?
                                        $sql = "SELECT crpid, crpdsc, usucpfobra, arqid, crpstatus
                                                FROM livro.camporespostaparecer
                                                WHERE cnpid = ".$p['cnpid'];
                                        $recurso = $db->pegaLinha($sql);

                                        if(!$recurso) {
                                            if( in_array( PNLD_PERFIL_EDITORA_CAMPO, $arrPerfil ) || $db->testa_superuser() ){?>
                                                <img style='cursor:hand' title='Adicionar arquivo' src='../imagens/incluir_p.gif' onclick="anexoRecurso('<?=$v['cobid']?>')">
                                            <?}else{?>
                                                -
                                            <?}
                                        }else{
                                            ?>
                                            <table width="90%" align="center" border="0" cellspacing="0" cellpadding="2">
                                                <tr>
                                                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor="">
                                                        <strong>Arquivo</strong>
                                                    </td>
                                                    <td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" bgcolor="">
                                                        <strong>Recurso</strong>
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align="center">
                                                        <?if( in_array( PNLD_PERFIL_EDITORA_CAMPO, $arrPerfil ) || $db->testa_superuser() ){?>
                                                            <img style='cursor:hand' title='Apagar arquivo' src='../imagens/excluir.gif' onclick="DelanexoRecurso('<?=$recurso['arqid']?>')">
                                                            &nbsp;
                                                        <?}?>
                                                        <img style='cursor:hand' title='Fazer download do arquivo' src='../imagens/clipe.gif' onclick="baixaranexoParecer('<?=$recurso['arqid']?>')">
                                                    </td>
                                                    <td align="center">
                                                        <textarea cols="20" rows="2" disabled><?=$recurso['crpdsc']?></textarea>
                                                    </td>
                                                </tr>
                                            </table>

                                            <?
                                        }
                                        ?>
                                    </td>
                                </tr>
                                <?
                            }
                            ?>
                            <tr>
                                <td align="center">
                                    <?if( in_array( PNLD_PERFIL_GESTOR_CAMPO, $arrPerfil ) || $db->testa_superuser() ){?>
                                        <img style='cursor:hand' title='Adicionar arquivo' src='../imagens/incluir_p.gif' onclick="anexoParecer('<?=$v['cobid']?>')">
                                    <?}else{?>
                                        -
                                    <?}?>
                                </td>
                                <td align="center">
                                    <?if( in_array( PNLD_PERFIL_EDITORA_CAMPO, $arrPerfil ) || $db->testa_superuser() ){?>
                                        <img style='cursor:hand' title='Adicionar arquivo' src='../imagens/incluir_p.gif' onclick="alert('É necessário o parecer do MEC primeiro.')">
                                    <?}else{?>
                                        -
                                    <?}?>
                                </td>
                            </tr>
                        </tbody>
                    </table>
                </td>
            </tr>
        <?}?>
    </tbody>
</table>



<?PHP 
######### Funções de UPLOAD #########
function EnviarArquivo($arquivo,$dados=null){
    global $db;
    
    if (!$arquivo){
        return false;
    }
    // obtém o arquivo
    #$arquivo = $_FILES['arquivo'];
    if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
        redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'], $parametros );
    }
    // BUG DO IE
    // O type do arquivo vem como image/pjpeg
    if($arquivo["type"] == 'image/pjpeg') {
            $arquivo["type"] = 'image/jpeg';
    }

    //Insere o registro do arquivo na tabela public.arquivo
    $sql = "
        INSERT INTO public.arquivo(
                    arqnome,
                    arqextensao,
                    arqdescricao,
                    arqtipo,
                    arqtamanho,
                    arqdata,
                    arqhora,
                    usucpf,
                    sisid
            )VALUES(
                    '".current(explode(".", $arquivo["name"]))."',
                    '".end(explode(".", $arquivo["name"]))."',
                    '".$dados["arqdescricao"]."',
                    '".$arquivo["type"]."',
                    '".$arquivo["size"]."',
                    '".date('Y/m/d')."',
                    '".date('H:i:s')."',
                    '".$_SESSION["usucpf"]."',
                    ". $_SESSION["sisid"] ."
            ) RETURNING arqid;
    ";
    $arqid = $db->pegaUm($sql);

    if(!is_dir('../../arquivos/livro/'.floor($arqid/1000))) {
        mkdir(APPRAIZ.'/arquivos/livro/'.floor($arqid/1000), 0777);
    }
    $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
    
    switch($arquivo["type"]) {
        case 'image/jpeg':
            ini_set("memory_limit", "128M");
            list($width, $height) = getimagesize($arquivo['tmp_name']);
            $original_x = $width;
            $original_y = $height;
            // se a largura for maior que altura
            if($original_x > $original_y) {
                    $porcentagem = (100 * 640) / $original_x;      
            }else {
                    $porcentagem = (100 * 480) / $original_y;  
            }
            $tamanho_x = $original_x * ($porcentagem / 100);
            $tamanho_y = $original_y * ($porcentagem / 100);
            $image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
            $image   = imagecreatefromjpeg($arquivo['tmp_name']);
            imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
            imagejpeg($image_p, $caminho, 100);
            //Clean-up memory
            ImageDestroy($image_p);
            //Clean-up memory
            ImageDestroy($image);
            break;
        default:
            if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
                $db->rollback();
                return false;
            }
    }
    $db->commit();
    return $arqid;
}

######## Fim funções de UPLOAD ########
function deletarAnexo($param){
    global $db;
    
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = {$param['arqid']}";
    $db->executar($sql);
    
    if($param['tipo'] == '1'){
    	$sql = "DELETE FROM livro.campoparecer WHERE arqid = {$param['arqid']}";
    }else{
    	$sql = "DELETE FROM livro.camporespostaparecer WHERE arqid = {$param['arqid']}";
    }
    $db->executar($sql);
    $db->commit();

    $caminho = APPRAIZ.'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($param['arqid']/1000) .'/'. $param['arqid'];
    if(file_exists($caminho)){
        unlink($caminho);
    }

    return true;
}

function DownloadArquivo($param){
    global $db;

    $sql ="SELECT * FROM public.arquivo WHERE arqid = ".$param['arqid'];
    $arquivo = $db->carregar($sql);
    
    $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
    
    if ( !is_file( $caminho ) ) {
        $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
    }
    $filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
    
    header( 'Content-type: '. $arquivo[0]['arqtipo'] );
    header( 'Content-Disposition: attachment; filename='.$filename);
    readfile( $caminho );
    exit();
}

?>