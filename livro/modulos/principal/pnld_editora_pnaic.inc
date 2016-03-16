<?php

include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

// Retorna um array com o código das responsabilidades vinculadas ao usuário logado.
$responsabilidade = retornaResponsabilidades($_SESSION['usucpf'], 'N', 'cod');

$resid = '2';

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
    $pncid = $_REQUEST['pncid'];
    showJsonArqAnexos($pncid);
    die();
}

if($_REQUEST['op'] == 'showDivArqAnexos'){
    $pncid = $_REQUEST['pncid'];
    showDivArqAnexos($pncid);
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
    $param["tipo"] = $_REQUEST['tipo'];
    deletarAnexo($param);
    unset($_REQUEST['arqid']);
    unset($_REQUEST['op']);
    
    die("
        <script>
            alert('Operação realizada com sucesso!');
            location.href='livro.php?modulo=principal/pnld_editora_pnaic&acao=A';
	</script>
    ");
}

//formulario para anexo
if ($_REQUEST['pncid']) {

    //anexa arquivo
    if ($_POST['submetido']) {
        if (isset($_POST['pncid'])) {
            if ($_FILES['arquivo']['size']) {
                $dados["pncid"] = $_REQUEST["pncid"];
                //$dados["resid"] = $_REQUEST["resid"];
                $dados["resid"] = $resid;
                $dados["tipo"] = $_REQUEST["tipo"];
                $dados["arqdescricao"] = 'CÓDIGO DA OBRA: ' . $dados["pncid"];

                //apaga
                if($_REQUEST["tipo"] == '1'){
	                $arqid = $db->pegaUm("select arqidparecer from livro.pnaiclivro where pncid = " . $_REQUEST['pncid']);
	                if ($arqid) {
	                    $param = array();
	                    $param["arqid"] = $arqid;
	                    $param["tipo"] = $_REQUEST["tipo"];
	                    deletarAnexo($param);
	                }
                }
                 
                if($_REQUEST["tipo"] == '2'){
	                $arqid = $db->pegaUm("select arqidrecurso from livro.pnaiclivro where pncid = " . $_REQUEST['pncid']);
	                if ($arqid) {
	                    $param = array();
	                    $param["arqid"] = $arqid;
	                    $param["tipo"] = $_REQUEST["tipo"];
	                    deletarAnexo($param);
	                }
                }
                
                //insere
                if (!$_FILES['arquivo']['size'] || EnviarArquivo($_FILES['arquivo'], $dados)) {
                    unset($_FILES['arquivo']['size']);
                    die("
                        <script>
                            alert('Operação realizada com sucesso!');
                            window.opener.location.href='livro.php?modulo=principal/pnld_editora_pnaic&acao=A';
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
        
        function anexarArquivo(tipo){
            var req  = document.getElementById('req');
            var form = document.getElementById('formA');

            if (document.getElementById('arquivo').value == '') {
                alert('Por favor selecione um arquivo no formato PDF.');
                return false;
            }
            if (form.resid.value == '' && tipo == 1) {
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
        <input type="hidden" id="pncid" name="pncid" value="<?= $_REQUEST['pncid'] ?>" />
        <input type="hidden" id="tipo" name="tipo" value="<?= $_REQUEST['tipo'] ?>" />
        <?if($_REQUEST['tipo'] == 2){?>
        	<input type="hidden" id="resid" name="resid" value="" />
        <?}?>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="3" cellPadding="5" align="center" width="100%">
            <tr>
                <td class="SubtituloTabela" align="center" colspan="2">
                    <?if($_REQUEST['tipo'] == 2){?>
                    	<b>Inserir Arquivo / Recurso</b>
                    <?}else{?>
                    	<b>Inserir Arquivo / Parecer</b>
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
                        if ($_REQUEST['pncid']) {
                            echo $db->pegaUm("select pnctitulo from livro.pnaiclivro where pncid = " . $_REQUEST['pncid']);
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
            <?if($_REQUEST['tipo'] == 1){?>
            <tr>
                <td align="right" class="SubTituloDireita">Parecer:</td>
                <td nowrap="nowrap">
                    <?PHP
                        $sql = "
                            SELECT  resid AS codigo,
                                    resdescricao AS descricao
                            FROM livro.pnaicresultado
                            ORDER BY 2
                        ";
                        $db->monta_combo("resid", $sql, 'N', "-- Informe o Parecer--", '', '', '', '', 'N', '', false, $resid);
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

monta_titulo('PNLD PNAIC', $linha2);

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
    
    function anexoParecer(pncid){
    	window.open("livro.php?modulo=principal/pnld_editora_pnaic&acao=A&tipo=1&pncid="+pncid,"pop","toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,left=600,top=200,width=530,height=250");
    }

    function anexoRecurso(pncid){
    	window.open("livro.php?modulo=principal/pnld_editora_pnaic&acao=A&tipo=2&pncid="+pncid,"pop","toolbar=no,location=no,status=no,menubar=no,scrollbars=no,resizable=no,left=600,top=200,width=530,height=250");
    }
    
    function DelanexoParecer(arqid){
    	if(confirm('Deseja realmente apagar este arquivo?')){
    		location.href="livro.php?modulo=principal/pnld_editora_pnaic&acao=A&tipo=1&op=delete&arqid="+arqid;
    	}
    }
    
  	function DelanexoRecurso(arqid){
    	if(confirm('Deseja realmente apagar este arquivo?')){
    		location.href="livro.php?modulo=principal/pnld_editora_pnaic&acao=A&tipo=2&op=delete&arqid="+arqid;
    	}
    }    

    function baixaranexoParecer(arqid){
    	location.href="livro.php?modulo=principal/pnld_editora_pnaic&acao=A&op=download&arqid="+arqid;
    }
    
    function addArqAnexosEja(pncid){
    	abreDivArqAnexos(pncid);
    }
    
    function abreDivArqAnexos(pncid){
        var url        = 'livro.php?modulo=principal/pnld_editora_pnaic&acao=A';
        var parametros = '&op=showDivArqAnexos'+'&pncid='+pncid;
        
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
                        salvaArquivoAnexo(pncid);
                });
                
                var divWidth  = 600;
                x = (window.innerWidth)/2 - (divWidth)/2;
                y = 100; 

                floatDiv("modalArqsAnexos", x,y).floatIt();
                
                carregaLinhasListaArquivos(pncid);
            }
        });
    }
    
    function excluirArquivoAnexo(pncid, arqid){
        
        var url        = 'livro.php?modulo=principal/pnld_editora_pnaic&acao=A';
        var parametros = '&op=deletarArqAnexo'+'&pncid='+pncid+"&arqid="+arqid;
        
        if(confirm('Deseja realmente apagar este arquivo?')){ 
            jQuery.ajax({
                url: url+parametros,
                async: false,
                success: function(html) {
                    if(html == 'true'){
                        alert('Arquivo excluído com sucesso!');
                        carregaLinhasListaArquivos(pncid);
                    }else{
                        alert('Falha ao excluir o arquivo!\n'+html);
                    }
                }
            });
    	}
    }
    
    function carregaLinhasListaArquivos(pncid){
        
        var perfil_ed_eja = <?php echo (checkPerfil(array(pnld_editora_pnaic), false)) ? 1 : 0 ; ?>;
        var url        = 'livro.php?modulo=principal/pnld_editora_pnaic&acao=A';
        var parametros = '&op=showJsonArqAnexos'+'&pncid='+pncid;
                
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
                                             '<img src="../imagens/excluir.gif" onclick="excluirArquivoAnexo('+pncid+','+arqid+')" title="Excluir" style="cursor:hand">' +
                                             '  </td>'+ 
                                             '  <td>'+ 
                                           '      <a href="#" onclick="baixaranexoParecer('+arqid+')">' +arqnome+ '</a>'+
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
                                           '      <a href="#" onclick="baixaranexoParecer('+arqid+')">' +arqnome+ '</a>'+
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
        //ver($responsabilidade);
        
        $stSql = "
            SELECT edt.edpid as codigo, 
                   edt.edpnomefantasia as descricao
            FROM livro.pnaiceditora edt";
        
		$stSql .= count($responsabilidade) > 0 ? " WHERE edt.edpid IN (" . implode(", ", $responsabilidade) . ")" : "";
        
        $stSql .= " order by 2";

        $stSqlCarregados = "";

        if (is_array($_POST['editora']) && $_POST['editora'][0]) {
            $stSqlCarregados = "
                SELECT edt.edpid as codigo,
                       edt.edpnomefantasia as descricao
                FROM livro.pnaiceditora edt
				WHERE  edt.edpid IN (" . implode(", ", $_POST['editora']) . ")";
            $stSqlCarregados .= " order by 2";
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
                <input type="button" name="Ver Todos" value="Ver Todos" onclick="location.href='livro.php?modulo=principal/pnld_editora_pnaic&acao=A';"/>
            </td>
        </tr>
    </table>
</form>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <?php
        
        extract($_POST);

        if ($editora[0] && ( $editora_campo_flag || $editora_campo_flag == '1' )) {
            $where[0] = " AND edi.edpid " . (( $editora_campo_excludente == null || $editora_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $editora) . "') ";
            $whereR[0] = " AND edi.edpid " . (( $editora_campo_excludente == null || $editora_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $editora) . "') ";
        }
        if ($descricao != '') {
            $where[3] = "AND edi.edpnomefantasia ilike '%" . $descricao . "%'";
            $whereR[3] = "AND edi.edpnomefantasia ilike '%" . $descricao . "%' OR obr.pnctitulo ilike '%" . $descricao . "%'";
        }


        $arrPerfil = arrayPerfil();
        if( in_array( PNLD_PERFIL_GESTOR_PNIAC, $arrPerfil ) || $db->testa_superuser() ){
            $btnAnexo = "<img style=\'cursor:hand\' title=\'Adicionar arquivo\' src=\'../imagens/incluir_p.gif\' onclick=\'anexoParecer('||pncid||')\'>";
            $btnDe = "&nbsp;&nbsp;&nbsp;&nbsp;<img style=\'cursor:hand\' title=\'Apagar arquivo\' src=\'../imagens/excluir.gif\' onclick=\'DelanexoParecer('||arqidparecer||')\'>";
            $btnAnexoRec = "<img style=\'cursor:hand\' title=\'Adicionar arquivo\' src=\'../imagens/incluir_p.gif\' onclick=\'anexoRecurso('||pncid||')\'>";
            $btnDeRec = "&nbsp;&nbsp;&nbsp;&nbsp;<img style=\'cursor:hand\' title=\'Apagar arquivo\' src=\'../imagens/excluir.gif\' onclick=\'DelanexoRecurso('||arqidrecurso||')\'>";
        } else {
            $btnAnexo = "";
            $btnDe = "";
            $btnAnexoRec = "";
            $btnDeRec = "";
        }
        
        $btnAnexoV = $btnAnexo != '' ? $btnAnexo : '-';
        
        $btnDo = "&nbsp;&nbsp;&nbsp;&nbsp;<img style=\'cursor:hand\' title=\'Fazer download do arquivo\' src=\'../imagens/clipe.gif\' onclick=\'baixaranexoParecer('||arqidparecer||')\'>";
        $btnDoRec = "&nbsp;&nbsp;&nbsp;&nbsp;<img style=\'cursor:hand\' title=\'Fazer download do arquivo\' src=\'../imagens/clipe.gif\' onclick=\'baixaranexoParecer('||arqidrecurso||')\'>";
        
        $sql = "select  pnccodigo,
                        pnctitulo,
                        edpnomefantasia,
                        pncautor,
                        pncano,
                        case when arqidparecer is null 
                            then '<center> $btnAnexoV </center>'
                            else '<center> <div style=width:83px;> $btnAnexo $btnDe $btnDo </div> </center>'
                        end as anexo,
                        resdescricao as parecer,
                        --case when arqidrecurso is null and obr.resid = 2 then '<center> $btnAnexoRec </center>'
                        	-- when arqidrecurso is not null and obr.resid = 2 then '<center> <div style=width:83px;> $btnAnexoRec $btnDeRec $btnDoRec </div> </center>'
                        	 --when arqidparecer is not null and obr.resid = 1 then '<center>-</center>'
                        --end as recurso,
                        pncid
                FROM  livro.pnaiclivro obr
                INNER JOIN livro.pnaiceditora edi ON edi.edpid = obr.edpid 
                LEFT JOIN livro.pnaicresultado res ON res.resid = obr.resid
                WHERE pncano = '" . $_SESSION['exercicio'] . "'
                ".$where[0]."
                ".$where[1]."
                ".$where[2]."
                ".$where[3]."
                " . (checkPerfil(array(pnld_editora_pnaic), false) ? (is_array($_SESSION['pnld']['edpid']) ? ' AND obr.edpid in (' . implode(',', $_SESSION['pnld']['edpid']) . ')' : '') : '' );
        
        $sql .= count($responsabilidade) > 0 ? " AND edi.edpid IN (" . implode(", ", $responsabilidade) . ")" : "";
        
        $sql .= " ORDER BY 1,2,3";
        $resultSet = $db->carregar($sql);        
        $resultSet = ($resultSet) ? $resultSet : array();   

        
        /*foreach ($resultSet as $key => $value) {
            $pncid = $resultSet[$key]['pncid'];
            unset($resultSet[$key]['pncid']);
            
            $sql_pnaicanexo = 'SELECT * FROM livro.pnaicanexo WHERE pncid = '.$pncid;
            $res_pnaicanexo = $db->carregar($sql_pnaicanexo);
            
            $str_pnaicanexo = "<img style=\"cursor:hand\" title=\"Visualizar arquivos\" src=\"../imagens/icone_lupa.png\" onclick=\"addArqAnexosEja('".$pncid."')\"> &nbsp;";
            
            $resultSet[$key]['arquivosanexo'] = $str_pnaicanexo;
            
            
        }*/
        
        //$cabecalho = array("Código", "Título", "Editora", "Autor", "Ano", "Arquivo", "Parecer", "Recurso", "Anexos");
        $cabecalho = array("Código", "Título", "Editora", "Autor", "Ano", "Arquivo", "Parecer");
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

    //atualiza o registro na tabela livro.pnaiclivro
    if($dados["tipo"]=='1'){
    	$sql = "UPDATE livro.pnaiclivro SET arqidparecer=$arqid, resid=".$dados["resid"]." WHERE pncid=".$dados["pncid"];
    }else{
    	$sql = "UPDATE livro.pnaiclivro SET arqidrecurso=$arqid WHERE pncid=".$dados["pncid"];
    }
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
    
    if($param['tipo'] == '1'){
    	$sql = "UPDATE livro.pnaiclivro SET arqidparecer = null, resid = null WHERE arqidparecer = {$param['arqid']}";
    }else{
    	$sql = "UPDATE livro.pnaiclivro SET arqidrecurso = null WHERE arqidrecurso = {$param['arqid']}";
    }
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
    
    $perfil_ed_eja = (checkPerfil(array(pnld_editora_pnaic), false)) ? 1 : 0 ; 
    
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
    
    $perfil_ed_eja = (checkPerfil(array(pnld_editora_pnaic), false)) ? 1 : 0 ; 
    
    if($perfil_ed_eja){
        return 'Você não possui permissão para inserir esse arquivo.';        
    }
    
    if ($_POST['submetido']) {
        if (isset($_POST['pncid'])) {
            if ($_FILES['arquivoAnexos']['size']) {
                $dados["pncid"]        = $_POST["pncid"];
                $dados["arqdescricao"] = $_POST['desanexo'];
                
                global $db;    
                $campos = array("pncid"	   => $_POST["pncid"],
                                "desanexo" => "'".$_POST['desanexo']."'");    
                $file   = new FilesSimec("pnaicanexo", $campos, 'livro');

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

function showDivArqAnexos($pncid){
    
    $chk = (checkPerfil(array(pnld_editora_pnaic), false)) ? 1 : 0 ;
    
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
        
        function salvaArquivoAnexo(pncid){

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
                                    carregaLinhasListaArquivos(pncid);
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
                <input type="hidden" id="pncid" name="pncid" value="'.$pncid .'" />
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

function showJsonArqAnexos($pncid){
    global $db;
    $sql          = 'SELECT * FROM livro.pnaicanexo WHERE pncid = '.$pncid;
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
