<?PHP
    header('Content-Type: text/html; charset=iso-8859-1');
    
    include "config.inc";    
    include APPRAIZ."includes/classes_simec.inc";
    include APPRAIZ."includes/funcoes.inc";
    
    $db = new cls_banco();
    $usucpf = $_REQUEST['usucpf'];
    $pflcod = $_REQUEST['pflcod'];

#INICIO REGISTRO RESPONSABILIDADES

if(is_array($_POST['edtSelecionadas'])) {
    $sql = "
        update livro.usuarioresponsabilidade Set rpustatus = 'I' where usucpf = '$usucpf' and pflcod = $pflcod 
    ";
    $db->executar($sql);

    if($_POST['edtSelecionadas'][0]){
        foreach($_POST['edtSelecionadas'] as $edtid){
            $sql = "
                insert into livro.usuarioresponsabilidade(
                        pflcod, usucpf,  rpustatus, rpudata_inc, edtid
                    )values(
                        $pflcod, '$usucpf', 'A', now(), '$edtid'
                )
            ";
            $db->executar($sql);
        }
    }
    $db->commit();
?>
    <script>
            window.parent.opener.location.reload();
            self.close();
    </script>
<?PHP
    exit();
}
#FIM REGISTRO RESPONSABILIDADES

?>
<html>
    <head>
        <META http-equiv="Pragma" content="no-cache">
        <title>Institui��o</title>
        <script language="JavaScript" src="/includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="/includes/Estilo.css">
        <link rel='stylesheet' type='text/css' href='/includes/listagem.css'>

    </head>

    <body LEFTMARGIN="0" TOPMARGIN="5" bottommargin="5" MARGINWIDTH="0" MARGINHEIGHT="0" BGCOLOR="#ffffff">

        <div align=center id="aguarde"><img src="/imagens/icon-aguarde.gif" border="0" align="absmiddle"> <font color=blue size="2">Aguarde! Carregando Dados...</font> </div>
        <? flush(); ?>

        <DIV style="OVERFLOW:AUTO; WIDTH:496px; HEIGHT:350px; BORDER:2px SOLID #ECECEC; background-color: White;">
            <form name="formulario">
                <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem" id="tabela">
                    <script language="JavaScript">
                        document.getElementById('tabela').style.visibility = "hidden";
                        document.getElementById('tabela').style.display  = "none";
                    </script>
                    <thead>
                        <tr>
                            <td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" colspan="4"><strong>Selecione a(s) Editora(s)</strong></td>
                        </tr>
                        <tr>
                            <td>&nbsp;</td>
                            <td><b>Codigo</b></td>
                            <td><b>Editora</b></td>
                        </tr>

<?PHP
    $cabecalho = 'Selecione a(s) Editora(ssss)';
    $sql = "
        SELECT  e.edtid,
                e.edtnome
        FROM livro.editora e

        --INNER JOIN livro.responsaveleditora re ON re.edtid=e.edtid AND reecpf='".$usucpf."'

        WHERE e.edtstatus = 'A'

        ORDER BY e.edtnome
    ";
    $RS = @$db->carregar($sql);

    if ($RS) {
        $nlinhas = count($RS) - 1;
        for ($i = 0; $i <= $nlinhas; $i++){
            extract($RS[$i]);
            if (fmod($i, 2) == 0)
                $cor = '#f4f4f4';
            else
                $cor = '#e0e0e0';
?>
            <tr bgcolor="<?= $cor ?>">
                <td align="right">
                    <input type="checkbox" name="usgid" id="<?= $edtid ?>" value="<?= $edtid ?>" onclick="retorna(<?= $i ?>);">
                    <input type="Hidden" name="edtnome" value="<?= $edtid . ' - ' . $edtnome ?>">
                </td>
                <td align="right" style="color:blue;"> <?= $edtid ?> </td>
                <td> <?= $edtnome ?> </td>
            </tr>
<?PHP
        }
    }else{
?>
        <tr>
            <td align="center" colspan="3" style="color: rgb(204, 0, 0);">N�o foram encontrados Registros.</td>
    	</tr>
<?PHP
    }
?>
        
                </table>
            </form>
        </div>  
        
        <form name="formassocia" style="margin:0px;" method="POST">
            <input type="hidden" name="usucpf" value="<?=$usucpf?>">
            <input type="hidden" name="pflcod" value="<?=$pflcod?>">
            
            <select multiple size="8" name="edtSelecionadas[]" id="edtSelecionadas" style="width:500px;" class="CampoEstilo" onchange="moveto(this);">
<?PHP
                $sql = "
                    SELECT DISTINCT e.edtid as codigo,
                            e.edtnome as descricao

                    FROM livro.usuarioresponsabilidade ur	

                    INNER JOIN livro.editora e ON (ur.edtid = e.edtid)

                    WHERE ur.rpustatus='A' AND ur.usucpf = '$usucpf' AND ur.pflcod = $pflcod
                ";
                $RS = @$db->carregar($sql);

                if(is_array($RS)) {
                    $nlinhas = count($RS)-1;
                    if ($nlinhas>=0) {
                        for ($i=0; $i<=$nlinhas;$i++) {
                            foreach($RS[$i] as $k=>$v) ${$k}=$v;

                            print " <option value=\"$codigo\">$codigo - $descricao</option>";
                        }
                    }
                } else {
?>
                    <option value="">Clique na Editora.</option>
<?PHP
                }
?>
                
            </select>
        </form>
        
        <table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
            <tr bgcolor="#c0c0c0">
                <td align="right" style="padding:3px;" colspan="3">
                    <input type="Button" name="ok" value="OK" onclick="selectAllOptions(campoSelect);document.formassocia.submit();" id="ok">
                </td>
            </tr>
        </table>
        
<script language="JavaScript">
    document.getElementById('aguarde').style.visibility = "hidden";
    document.getElementById('aguarde').style.display  = "none";
    document.getElementById('tabela').style.visibility = "visible";
    document.getElementById('tabela').style.display  = "";

    var campoSelect = document.getElementById("edtSelecionadas");

    if (campoSelect.options[0].value != ''){
	for(var i=0; i<campoSelect.options.length; i++){
            document.getElementById(campoSelect.options[i].value).checked = true;
	}
    }

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
	//alert(objeto);
	tamanho = campoSelect.options.length;
        
	if (campoSelect.options[0].value=='') {
            tamanho--;
	}
	//alert(tamanho);
	var arUsgid = document.getElementsByName( 'usgid' );
	var arEdtnome = document.getElementsByName( 'edtnome' );

	if (arUsgid[objeto].checked == true){
            campoSelect.options[tamanho] = new Option(arEdtnome[objeto].value, arUsgid[objeto].value, false, false);
            sortSelect(campoSelect);
	} else {
            for(var i=0; i<=campoSelect.length-1; i++){
                if (arUsgid[objeto].value == campoSelect.options[i].value)
                    {campoSelect.options[i] = null;}
                }
                if (!campoSelect.options[0]){
                    campoSelect.options[0] = new Option('Clique na Editora.', '', false, false);
                }
                sortSelect(campoSelect);
	}
    }

    function moveto(obj) {
        /*if (obj.options[0].value != '') {
            if(document.getElementById('img'+obj.value.slice(0,obj.value.indexOf('.'))).name=='+'){
                abreconteudo(obj.value.slice(0,obj.value.indexOf('.')));
            }
            document.getElementById(obj.value).focus();
        }*/
    }
</script>