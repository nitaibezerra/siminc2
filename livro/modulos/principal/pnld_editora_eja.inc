<?php

include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

if ($_REQUEST['op'] == 'download' && $_REQUEST['arqid']) {
    $param = array();
    $param["arqid"] = $_REQUEST['arqid'];
    DownloadArquivo($param);
    exit;
}

if($_REQUEST['op'] == 'uploadArqAnexos'){
    uploadArqAnexos();
    die();
}

if($_REQUEST['op'] == 'showJsonArqAnexos'){
    $ejaid = $_REQUEST['ejaid'];
    showJsonArqAnexos($ejaid);
    die();
}

if($_REQUEST['op'] == 'showDivArqAnexos'){
    $ejaid = $_REQUEST['ejaid'];
    showDivArqAnexos($ejaid);
    die();
}
if($_REQUEST['op'] == 'deletarArqAnexo'){    
    $param["arqid"] = $_REQUEST['arqid'];
    $resp = deletarArqAnexo($param);
    echo $resp;
    die();
}

if ($_REQUEST['op'] == 'delete' && $_REQUEST['arqid']) {
    $param = array();
    $param["arqid"] = $_REQUEST['arqid'];
    deletarAnexo($param);
    unset($_REQUEST['arqid']);
    unset($_REQUEST['op']);
    
    die("
        <script>
            alert('Operação realizada com sucesso!');
            location.href='livro.php?modulo=principal/pnld_editora_eja&acao=A';
	</script>
    ");
}

//formulario para anexo
if ($_REQUEST['ejaid']) {

    //anexa arquivo
    if ($_POST['submetido']) {
        if (isset($_POST['ejaid'])) {
            if ($_FILES['arquivo']['size']) {
                $dados["ejaid"] = $_REQUEST["ejaid"];
                $dados["resid"] = $_REQUEST["resid"];
                $dados["arqdescricao"] = 'CÓDIGO DA EJA OBRA: ' . $dados["ejaid"];

                //apaga
                $arqid = $db->pegaUm("select arqid from livro.ejaobra where ejaid = " . $_REQUEST['ejaid']);
                if ($arqid) {
                    $param = array();
                    $param["arqid"] = $arqid;
                    deletarAnexo($param);
                }

                //insere
                if (!$_FILES['arquivo']['size'] || EnviarArquivo($_FILES['arquivo'], $dados)) {
                    unset($_FILES['arquivo']['size']);
                    die("
                        <script>
                            alert('Operação realizada com sucesso!');
                            window.opener.location.href='livro.php?modulo=principal/pnld_editora_eja&acao=A';
                            self.close();
                        </script>
                    ");
                }else{
                    die("
                        <script>
                            alert(\"Problemas no envio do arquivo.\");
                            history.go(-1);
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
        
        function anexarArquivo(){
            var req  = document.getElementById('req');
            var form = document.getElementById('formA');

            if (document.getElementById('arquivo').value == '') {
                alert('Por favor selecione um arquivo no formato PDF.');
                return false;
            }
            if (form.resid.value == '') {
                alert('Por favor informe o parecer.');
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
        <input type="hidden" id="ejaid" name="ejaid" value="<?= $_REQUEST['ejaid'] ?>" />
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="3" cellPadding="5" align="center" width="100%">
            <tr>
                <td class="SubtituloTabela" align="center" colspan="2">
                    <b>Inserir Arquivo / Parecer</b>
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
                        if ($_REQUEST['ejaid']) {
                            echo $db->pegaUm("select ejatitulo from livro.ejaobra where ejaid = " . $_REQUEST['ejaid']);
                        }
                    ?>
                </td>
            </tr>
            <tr>
                <td align="right" class="SubTituloDireita" style="width:100px;">Arquivo:</td>
                <td>
                    <input type="file" id="arquivo" name="arquivo" />
                    <img border="0" src="/imagens/obrig.gif" />
                </td>
            </tr>
            <tr>
                <td align="right" class="SubTituloDireita">Parecer:</td>
                <td nowrap="nowrap">
                    <?PHP
                        $sql = "
                            SELECT  resid AS codigo,
                                    resdescricao AS descricao
                            FROM livro.ejaresultado
                            ORDER BY 2
                        ";
                        $db->monta_combo("resid", $sql, 'S', "-- Informe o Parecer--", '', '', '', '', 'S');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita"></td>
                <td align="left">
                    <input type="button" value="Salvar" id="btSalvar" onclick="anexarArquivo()">
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

monta_titulo('PNLD EJA', $linha2);

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
            selectAllOptions(formulario.segmento);
            selectAllOptions(formulario.colecao);
            //selectAllOptions(formulario.parecer);
        } else {
            formulario.descricao.value = '';
        }
        formulario.submit();
    }
    
    function anexoEja(ejaid){
    	window.open("livro.php?modulo=principal/pnld_editora_eja&acao=A&ejaid="+ejaid,"pop","toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,left=600,top=200,width=530,height=250");
    }
    
    function addArqAnexosEja(ejaid){
    	abreDivArqAnexos(ejaid);
    }
    
    function DelAnexoEja(arqid){
    	if(confirm('Deseja realmente apagar este arquivo?')){
    		location.href="livro.php?modulo=principal/pnld_editora_eja&acao=A&op=delete&arqid="+arqid;
    	}
    }

    function baixarAnexoEja(arqid){
    	location.href="livro.php?modulo=principal/pnld_editora_eja&acao=A&op=download&arqid="+arqid;
    }
    
    function abreDivArqAnexos(ejaid){
        var url        = 'livro.php?modulo=principal/pnld_editora_eja&acao=A';
        var parametros = '&op=showDivArqAnexos'+'&ejaid='+ejaid;
        
        var div = '<div id="modalArqsAnexos"            '+
                  '     style="display:  none;                   '+
                  '            background-color: #FFF;           '+
                  '            width:    600;                    '+
                  '            height:   450;                    '+
                  '            position: absolute;               '+
                  '            box-shadow: 7px 7px 5px #A8A9A9;  '+
                  '            opacity: none;                    '+ 
                  '            filter:alpha(opacity=none);       '+
                  '           -khtml-opacity:1;                  '+ 
                  '            -moz-opacity:1;                   '+ 
                  '            -ms-filter:?alpha(opacity=none)?; '+
                  '            filter: progid:DXImageTransform.Microsoft.Alpha(opacity=none); '+
                  '            padding-top: 0;                   '+
                  '            -webkit-border-radius: 7px;      '+
                  '            -moz-border-radius: 7px;         '+
                  '             border-radius: 7px;             '+
                  '            overflow: auto;                   '+
                  '            z-index:  900; ">&nbsp;</div>     ';
          
        var div_background = '<div id="div_dialog_justificativa_bg"       '+
                             '     style=" background-color: #000;        '+ 
                             '             bottom: 0;                     '+ 
                             '             display: none;                 '+ 
                             '             left: 0;                       '+ 
                             '             opacity: 0.5;                  '+ 
                             '             filter:alpha(opacity=50);       '+
                             '            -khtml-opacity:.50;              '+ 
                             '            -moz-opacity:.50;               '+ 
                             '            -ms-filter:?alpha(opacity=50)?; '+
                             '             filter: progid:DXImageTransform.Microsoft.Alpha(opacity=0.5); '+
                             '             position: fixed; '+ 
                             '             right: 0;        '+ 
                             '             top: 0;          '+ 
                             '             z-index: 99;     '+ 
                             '            ">&nbsp;</div>    ';
        
        jQuery.ajax({
            url: url+parametros,
            async: false,
            success: function(html) {
                if(jQuery("#div_dialog_justificativa_bg").length === 0){
                    jQuery('body').append(div_background);
                }

                jQuery("#div_dialog_justificativa_bg").show();
                jQuery('body').append(div);

                jQuery("#modalArqsAnexos").append(html);
                jQuery("#modalArqsAnexos").show();
                
                jQuery(".btnFechar").click(function(){
                    jQuery("#modalArqsAnexos").remove();
                    jQuery("#div_dialog_justificativa_bg").hide();
                });

                jQuery(".btnSalvar").click(function(){                        
                        salvaArquivoAnexo(ejaid);
                });
                
                var divWidth  = 600;
                x = (window.innerWidth)/2 - (divWidth)/2;
                y = 100; 

                floatDiv("modalArqsAnexos", x,y).floatIt();
                
                carregaLinhasListaArquivos(ejaid);
            }
        });
    }
    
    function excluirArquivoAnexo(ejaid, arqid){
        
        var url        = 'livro.php?modulo=principal/pnld_editora_eja&acao=A';
        var parametros = '&op=deletarArqAnexo'+'&ejaid='+ejaid+"&arqid="+arqid;
        
        if(confirm('Deseja realmente apagar este arquivo?')){ 
            jQuery.ajax({
                url: url+parametros,
                async: false,
                success: function(html) {
                    if(html == 'true'){
                        alert('Arquivo excluído com sucesso!');
                        carregaLinhasListaArquivos(ejaid);
                    }else{
                        alert('Falha ao excluir o arquivo!\n'+html);
                    }
                }
            });
    	}
    }
    
    function carregaLinhasListaArquivos(ejaid){
        
        var perfil_ed_eja = <?php echo (checkPerfil(array(PNLD_EDITORA_EJA), false)) ? 1 : 0 ; ?>;
        var url        = 'livro.php?modulo=principal/pnld_editora_eja&acao=A';
        var parametros = '&op=showJsonArqAnexos'+'&ejaid='+ejaid;
                
        jQuery.ajax({
                url: url+parametros,
                async: false,
                success: function(json) {

                    var arqid   = '';
                    var arqnome = '';
                    var arqdesc = '';
                    var tr_add  = '';
                    var obj     = '';
                    
                    jQuery('#lista_arquivos > tbody').html('');

                    if(json.length > 0){
                        for(var i in json){

                            obj     = json[i];
                            arqid   = obj.arqid;
                            arqnome = obj.arqnome;
                            arqdesc = obj.arqdesc;

                            if(arqid !== undefined && arqid !== 'undefined'){
                                if(!perfil_ed_eja){
                                    tr_add = '<tr>'+ 
                                             '  <td>'+ 
                                             '<img src="../imagens/excluir.gif" onclick="excluirArquivoAnexo('+ejaid+','+arqid+')" title="Excluir" style="cursor:hand">' +
                                             '  </td>'+ 
                                             '  <td>'+ 
                                           '      <a href="#" onclick="baixarAnexoEja('+arqid+')">' +arqnome+ '</a>'+
                                             '  </td>'+ 
                                             '  <td>'+ 
                                             '      ' +arqdesc+ ''+
                                             '  </td>'+ 
                                             '</tr>';
                                }else{
                                    tr_add = '<tr>'+ 
                                             '  <td>'+ 
                                             '  &nbsp  ' +
                                             '  </td>'+ 
                                             '  <td>'+ 
                                           '      <a href="#" onclick="baixarAnexoEja('+arqid+')">' +arqnome+ '</a>'+
                                             '  </td>'+ 
                                             '  <td>'+ 
                                             '      ' +arqdesc+ ''+
                                             '  </td>'+ 
                                             '</tr>';
                                 }
                                
                                 
                                jQuery('#lista_arquivos > tbody').append(tr_add);
                            }
                        }                                
                    }

                }
            });
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
        
        $stSql = "
            SELECT segid as codigo,
                   segdescricao as descricao
            FROM livro.ejasegmento
        ";
        
        $stSqlCarregados = "";
        if (is_array($_POST['segmento']) && $_POST['segmento'][0]) {
            $stSqlCarregados = "
                SELECT segid as codigo, 
                       segdescricao as descricao
                FROM livro.ejasegmento
                WHERE segid IN (" . implode(", ", $_POST['segmento']) . ")
            ";
        }
        mostrarComboPopup('Segmento', 'segmento', $stSql, $stSqlCarregados, 'Selecione o segmento');

        $stSql = "
            SELECT colid as codigo,
                   colcodigoid || ' - ' || col.coldescricao as descricao
            FROM livro.ejacolecao col
            WHERE colano = '" . $_SESSION['exercicio'] . "';
        ";
        
        $stSqlCarregados = "";
        if (is_array($_POST['colecao']) && $_POST['colecao'][0]) {
            $stSqlCarregados = "
                SELECT colid as codigo,
                       coldescricao as descricao
                FROM livro.ejacolecao col
                WHERE colid IN (" . implode(", ", $_POST['colecao']) . ") and colano = '" . $_SESSION['exercicio'] . "';
            ";
        }
        mostrarComboPopup('Coleção', 'colecao', $stSql, $stSqlCarregados, 'Selecione a Coleção');
        
        $stSql = "
            SELECT edt.ediid as codigo, 
                   edt.edinome as descricao
            FROM livro.ejaeditora edt
            order by 2
        ";

        $stSqlCarregados = "";

        if (is_array($_POST['editora']) && $_POST['editora'][0]) {
            $stSqlCarregados = "
                SELECT edt.ediid as codigo,
                       edt.edinome as descricao
                FROM livro.ejaeditora edt
                WHERE  edt.ediid IN (" . implode(", ", $_POST['editora']) . ")
                order by 2
            ";
        }
        mostrarComboPopup('Editora', 'editora', $stSql, $stSqlCarregados, 'Selecione a Editora');

?>
        <tr>
            <td class="SubTituloDireita" width="10%">Descrição:</td>
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
                <input type="button" name="Ver Todos" value="Ver Todos" onclick="javascript:gerarRelatorio(2);"/>
            </td>
        </tr>
    </table>
</form>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <?php
        
        extract($_POST);

        if ($editora[0] && ( $editora_campo_flag || $editora_campo_flag == '1' )) {
            $where[0] = " AND edi.ediid " . (( $editora_campo_excludente == null || $editora_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $editora) . "') ";
            $whereR[0] = " AND edi.ediid " . (( $editora_campo_excludente == null || $editora_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $editora) . "') ";
        }
        if ($segmento[0] && ( $segmento_campo_flag || $segmento_campo_flag == '1' )) {
            $where[1] = " AND seg.segid " . (( $segmento_campo_excludente == null || $segmento_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $segmento) . "') ";
            $whereR[1] = " AND seg.segid " . (( $segmento_campo_excludente == null || $segmento_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $segmento) . "') ";
        }
        if ($colecao[0] && ( $colecao_campo_flag || $colecao_campo_flag == '1' )) {
            $where[2] = " AND col.colid " . (( $colecao_campo_excludente == null || $colecao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $colecao) . "') ";
            $whereR[2] = " AND 1 = 2 ";
        }
        if ($descricao != '') {
            $where[3] = "AND edi.edinome ilike '%" . $descricao . "%' OR seg.segdescricao ilike '%" . $descricao . "%' OR col.coldescricao ilike '%" . $descricao . "%'";
            $whereR[3] = "AND edi.edinome ilike '%" . $descricao . "%' OR seg.segdescricao ilike '%" . $descricao . "%' OR obr.ejatitulo ilike '%" . $descricao . "%'";
        }


        $arrPerfil = arrayPerfil();
        if( in_array( PNLD_PERFIL_GESTOR_EJA, $arrPerfil ) || $db->testa_superuser() ){
            $btnAnexo = "<img style=\'cursor:hand\' title=\'Adicionar arquivo\' src=\'../imagens/incluir_p.gif\' onclick=\'anexoEja('||ejaid||')\'>";
            $btnDe = "&nbsp;&nbsp;&nbsp;&nbsp;<img style=\'cursor:hand\' title=\'Apagar arquivo\' src=\'../imagens/excluir.gif\' onclick=\'DelAnexoEja('||arqid||')\'>";
        } else {
            $btnAnexo = "-";
            $btnDe = "";
        }
        
        $btnDo = "&nbsp;&nbsp;&nbsp;&nbsp;<img style=\'cursor:hand\' title=\'Fazer download do arquivo\' src=\'../imagens/clipe.gif\' onclick=\'baixarAnexoEja('||arqid||')\'>";
        
        $sql = "select  ejacodigo,
                        ejatitulo,
                        ejanumeroedicao,
                        segdescricao,
                        coldescricao,
                        edinome,
                        ejaautor,
                        ejaano,
                        case when arqid is null 
                            then '<center> $btnAnexo </center>'
                            else '<center> <div style=width:83px;> $btnAnexo $btnDe $btnDo </div> </center>'
                        end as anexo,
                        resdescricao as parecer,
                        ejaid
                FROM  livro.ejaobra obr
                INNER JOIN livro.ejasegmento seg ON seg.segid = obr.segid
                INNER JOIN livro.ejacolecao col ON col.colid = obr.colid
                INNER JOIN livro.ejaeditora edi ON edi.ediid = obr.ediid 
                LEFT JOIN livro.ejaresultado res ON res.resid = obr.resid
                WHERE colano = '" . $_SESSION['exercicio'] . "'
                ".$where[0]."
                ".$where[1]."
                ".$where[2]."
                ".$where[3]."
                " . (checkPerfil(array(PNLD_EDITORA_EJA), false) ? (is_array($_SESSION['pnld']['ediid']) ? ' AND obr.ediid in (' . implode(',', $_SESSION['pnld']['ediid']) . ')' : '') : '' ) . "
                ORDER BY 1,2,3
        ";
        $resultSet = $db->carregar($sql);        
        $resultSet = ($resultSet) ? $resultSet : array();        
        foreach ($resultSet as $key => $value) {
            $ejaid = $resultSet[$key]['ejaid'];
            unset($resultSet[$key]['ejaid']);
            
            $sql_ejaanexo = 'SELECT * FROM livro.ejaanexo WHERE ejaid = '.$ejaid;
            $res_ejaanexo = $db->carregar($sql_ejaanexo);
            
            $str_ejaanexo = "<img style=\"cursor:hand\" title=\"Visualizar arquivos\" src=\"../imagens/icone_lupa.png\" onclick=\"addArqAnexosEja('".$ejaid."')\"> &nbsp;";
            
            $resultSet[$key]['arquivosanexo'] = $str_ejaanexo;
        }
        
        $cabecalho = array("Código", "Título", "Edição", "Segmento", "Coleção", "Editora", "Autor", "Ano", "Arquivo", "Parecer", "Anexos");
        $db->monta_lista_array($resultSet, $cabecalho, 50, 20, 'N', 'center');
        
?>
    </tr>
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

    //atualiza o registro na tabela livro.ejaobra
    $sql = "UPDATE livro.ejaobra SET arqid=$arqid, resid=".$dados["resid"]." WHERE ejaid=".$dados["ejaid"];
    $db->executar($sql);

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
    return true;
}

######## Fim funções de UPLOAD ########
function deletarAnexo($param){
    global $db;
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = {$param['arqid']}";
    $db->executar($sql);
    
    $sql = "UPDATE livro.ejaobra SET arqid = null, resid = null WHERE arqid = {$param['arqid']}";
    $db->executar($sql);
    
    $db->commit();
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

function deletarArqAnexo($param){
    global $db;
    
    $perfil_ed_eja = (checkPerfil(array(PNLD_EDITORA_EJA), false)) ? 1 : 0 ; 
    
    if($perfil_ed_eja){
        return 'Você não possui permissão para excluir esse arquivo.';        
    }
    
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid = {$param['arqid']}";    
    try{
        $db->executar($sql);    
        $db->commit();        
        return 'true';
    } catch (Exception $ex) {
        $db->rollback();
//        return 'Falha ao excluir o arquivo. \nERRO: '.$ex->getMessage();
        return 'Falha ao excluir o arquivo.';
    }
}

function uploadArqAnexos(){
    
    $perfil_ed_eja = (checkPerfil(array(PNLD_EDITORA_EJA), false)) ? 1 : 0 ; 
    
    if($perfil_ed_eja){
        return 'Você não possui permissão para inserir esse arquivo.';        
    }
    
    if ($_POST['submetido']) {
        if (isset($_POST['ejaid'])) {
            if ($_FILES['arquivoAnexos']['size']) {
                $dados["ejaid"]        = $_POST["ejaid"];
                $dados["arqdescricao"] = $_POST['desanexo'];
                
                global $db;    
                $campos = array("ejaid"	   => $_POST["ejaid"],
                                "desanexo" => "'".$_POST['desanexo']."'");    
                $file   = new FilesSimec("ejaanexo", $campos, 'livro');

                if($_FILES["arquivoAnexos"]){	
                    $arquivoSalvo = $file->setUpload($_POST['desanexo'],"arquivoAnexos", TRUE, TRUE);	
                    if($arquivoSalvo){
                        $arqid = $file->getIdArquivo();
                    }else{
                        $arqid = false;
                    }
                }

                if(!$arqid){
                    $db->rollback();
                    echo 'Erro ao realizar o Upload do Arquivo';
                    die();
                }else{
                    $db->commit();
                    echo 'Operação realizada com sucesso!';
                    
                }
            }
        }        
    }
    die();
}

function showDivArqAnexos($ejaid){
    
    $chk = (checkPerfil(array(PNLD_EDITORA_EJA), false)) ? 1 : 0 ;
    
    if(!$chk){
        $titulo = 'Inserir Arquivo(s) Anexo(s)';
    }else{
        $titulo = 'Listar Arquivo(s) Anexo(s)';
    }
    
    $html  = ' 
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <!--<link rel="stylesheet" type="text/css" href="../includes/layout/pdeinterativo/css/simec.css"/>-->
    
    <script type="text/javascript" src="../../includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
    <script type="text/javascript" src="/library/jquery/jquery.form.min.js"></script>

    <script type="text/javascript">
    
        $(document).ready(function(){});
        
        function salvaArquivoAnexo(ejaid){

            var options = { beforeSend: function() {
                                $("#progress").show();
                                //clear everything
                                $("#bar").width(\'0%\');
                                $("#message").html("");
                                $("#percent").html("0%");
                            },
                            uploadProgress: function(event, position, total, percentComplete) {
                                $("#bar").width(percentComplete+\'%\');
                                $("#percent").html(percentComplete+\'%\');

                            },
                            success: function(){
                                $("#bar").width(\'100%\');
                                $("#percent").html(\'<b>100%</b>\');
                            },
                            complete: function(response){
                                setTimeout(function(){
                                    $("#percent").attr(\'style\',\'left:25% ;text-align:center;\');
                                    $("#percent").html("<font color=\'green\'><b>"+response.responseText+"</b></font>");
                                },300);
                                setTimeout(function(){
                                    $("#progress").hide();
                                },3000);
                                setTimeout(function(){
                                    carregaLinhasListaArquivos(ejaid);
                                },500);
                            },
                            error: function(){
                                $("#percent").attr(\'style\',\'left:25% ;text-align:center;\');
                                $("#percent").html("<font color=\'red\'> ERROR: unable to upload files</font>");
                            }
                          }; 


            if(verificaExtensaoEditora(\'arquivoAnexos\') && $("#arquivoAnexos").val() !== \'\'){
                $("#formAA").ajaxForm(options);
                $("#formAA").submit();
            }else{
                alert(\'Preencha o campo de arquivo.\');
            }

        }
        
        function verificaExtensaoEditora(id) {
            arquivo = document.getElementById(id);
            if (arquivo.value != \'\') {
                arFile = arquivo.value.split(\'.\');
                indice = arFile.length - 1;
                extensao = arFile[indice].toLowerCase();
                if (extensao != \'pdf\') {
                    alert(\'Extensão não permitida! Envie apenas arquivo com extensão PDF.\');
                    arquivo.value = \'\';
                    return false;
                }
            }
            return true;
        }
        
    </script>
    
    <style>
        #progress { position:relative; width:400px; border: 1px solid #ddd; padding: 1px; border-radius: 3px; }
        #bar { background-color: #B4F5B4; width:0%; height:20px; border-radius: 3px; }
        #percent { position:absolute; display:inline-block; top:3px; left:48%; }
    </style>
    
    <table class="tabela" bgcolor="#fff" cellSpacing="3" cellPadding="5" align="center" width="100%">
        <tr>
            <td class="SubtituloTabela" align="center" style="width:5%;">
                &nbsp;
            </td>
            <td class="SubtituloTabela" align="center" style="width:90%;">
                <b>'.$titulo.'</b>
            </td>
            <td class="SubtituloTabela" align="right" style="width:5%; font-size:15px; color:#ccc; padding-right: 15px;">
                <b><span class="btnFechar" style="cursor:hand" title="Fechar">X</b>
            </td>
        </tr>
    </table>';
    
    if(!$chk){
        $html .= ' 
            <form id="formAA" method="post" enctype="multipart/form-data" action="">
                <input type="hidden" id="submetido" name="submetido" value="1" />
                <input type="hidden" id="op" name="op" value="uploadArqAnexos" />
                <input type="hidden" id="ejaid" name="ejaid" value="'.$ejaid .'" />
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="3" cellPadding="5" align="center" width="100%">
                    <!--<tr>
                        <td class="SubtituloTabela" align="center" colspan="2">
                            <b>Inserir Arquivo(s) Anexo(s)</b>
                        </td>
                    </tr>-->
                    <tr>
                        <td bgcolor="" align="center" colspan="2">
                            <img border="0" src="/imagens/obrig.gif" /> Indica campo obrigatório
                        </td>
                    </tr>

                    <tr>
                        <td align="right" class="SubTituloDireita" style="width:100px;">Arquivo:</td>
                        <td>
                            <input type="file" id="arquivoAnexos" name="arquivoAnexos" /> &nbsp; <img border="0" src="/imagens/obrig.gif" />
                        </td>
                    </tr>
                    <tr>
                        <td align="right" class="SubTituloDireita">Descrição do arquivo:</td>
                        <td nowrap="nowrap">
                            <input type="text" maxlength="50" size="50" value="" name="desanexo" id="desanexo" />
                        </td>
                    </tr>
                    <tr>
                        <td align="center" colspan="2">
                            <input type="button" value="Salvar" class="btnSalvar" >
                            <input type="button" value="Fechar" class="btnFechar" >
                        </td>
                    </tr>
                </table>

                <center>
                <div id="progress" style="display:none" style="width:100%;" align="center">
                        <div id="bar"></div>
                        <div id="percent">0%</div >
                </div>
                </center>
                <div id="message"></div>
            </form>
                      ' ;
    }
    
    $html .= ' 
                <table class="tabela" id="lista_arquivos" bgcolor="#f5f5f5" cellSpacing="3" cellPadding="5" align="center" width="100%">
                    <thead>
                    <tr>
                        <td class="SubtituloTabela" align="center" colspan="3">
                            <b>Lista de Arquivos Anexos</b>
                        </td>
                    </tr>
                    <tr>
                        <th align="center" style="width: 50px">
                            <b>Ação</b>
                        </th>
                        <th align="center">
                            <b>Arquivo</b>
                        </th>
                        <th align="center">
                            <b>Descrição</b>
                        </th>
                    </tr>
                    </thead>
                    <tbody>
                    </tbody>
                </table>
             ';
    
    echo $html;
}

function showJsonArqAnexos($ejaid){
    global $db;
    $sql          = 'SELECT * FROM livro.ejaanexo WHERE ejaid = '.$ejaid;
    $arquivos     = $db->carregar($sql);    
    $arquivos     = (is_array($arquivos)) ? $arquivos : array();    
    $arr_arquivos = array();    
    foreach ($arquivos as $key => $arq) {        
        $sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arq['arqid']. " AND arqstatus = 'A'";
        $arquivo = $db->pegaLinha($sql);
        if(is_array($arquivo)){
            $filename = $arquivo['arqnome'].'.'.$arquivo['arqextensao'];        
            $arr_arquivos[] = array('arqid'=>$arq['arqid'],'arqnome'=>$filename,'arqdesc'=>trim($arq['desanexo']));            
        }        
    }    
    header('Content-type: application/json');
    echo simec_json_encode($arr_arquivos);    
}


?>
