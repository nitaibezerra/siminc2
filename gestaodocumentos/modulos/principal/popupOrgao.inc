<?php 
if($_REQUEST['requisicao']) {        
	$_REQUEST['requisicao']($_REQUEST);
}

function pesquisarOrgao(){
    global $db; 
    header('content-type: text/html; charset=ISO-8859-1'); 

    extract($_POST);

    if($ogsdsc) {
        $ogsdsc = removeAcentos(trim($ogsdsc));
        $aryWhere[] = "public.removeacento(o.ogsdsc) ILIKE ('%{$ogsdsc}%') ";
    }

    if($tpoid) {
        $aryWhere[] = "t.tpoid = {$tpoid} ";
    }
    
    if($tpsid) {
        $aryWhere[] = "t.tpsid = {$tpsid} ";
    }    

    $sql = "SELECT  		'<input type=\"radio\" name=\"ogsid\" id=\"ogsid_'||o.ogsid||'\" value=\"'||o.ogsid||'\" onclick=\"salvar();\">' as acao,
                    		t.tpodsc,
                    		o.ogsdsc
            FROM 			gestaodocumentos.orgaosolicitante AS o
            INNER JOIN 		gestaodocumentos.tipoorgao AS t ON t.tpoid = o.tpoid
            				".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
            ORDER BY 		o.ogsdsc";
    
    $cabecalho = array("Ação", "Tipo", "Descrição");  
    $alinhamento = Array('center', 'left', 'left');
    $tamanho = Array('3%', '30%', '40%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
    die();
}	

function salvar(){
	global $db; 	
	header('content-type: text/html; charset=ISO-8859-1'); 
	
	$sql = "SELECT  	o.ogsid,
                    	o.ogsdsc,
                    	t.tpodsc
            FROM 		gestaodocumentos.orgaosolicitante AS o
            INNER JOIN  gestaodocumentos.tipoorgao AS t ON t.tpoid = o.tpoid
            WHERE 		o.ogsid = {$_POST['ogsid']}";
    
	$rs  = $db->carregar($sql);

    if($rs){
    	$_SESSION['iescodSessionEntidade'] = $_POST['ogsid'];
        $resp = "<span style=\"color: #1874CD;\"\">".$rs[0]['ogsid']." - ".$rs[0]['tpodsc']." - ".$rs[0]['ogsdsc']."</span>";
        $resp .= "<input type=\"hidden\" name=\"ogsid\" id=\"ogsid\" value=\"{$rs[0]['ogsid']}\" />";
        die($resp);
    }	
}
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<br>    
<form name="formulario" id="formulario" method="POST">
    <input type="hidden" name="requisicao" id="requisicao" value=""/>
    <input type="hidden" name="tarid" id="tarid" value="" />    
    <input type="hidden" name="ogsid" id="ogsid" value="" />
    <input type="hidden" name="tpsid" id="tpsid" value="<?php echo $_REQUEST['tpsid']; ?>" />  
    <table  bgcolor="#f5f5f5" align="center" class="tabela" >
        <tr>
            <td class = "subtitulodireita" colspan="2" style="text-align: center; font-size: 12px;"><h3>Busca de instituição</h3></td> 
        </tr>
        <tr>
            <td class = "subtitulodireita" width="33%">Tipo de Orgão:</td>
            <td>
                <?php 
                    $sql = "
                        SELECT  tpoid as codigo, 
                                tpodsc as descricao 
                        FROM gestaodocumentos.tipoorgao
                        WHERE tpsid = {$_REQUEST['tpsid']}
                        ORDER BY tpodsc
                    ";
                    $db->monta_combo("tpoid", $sql, 'S', 'Todos', '', '', '', '420', 'S', 'tpoid', false, null, 'Tipo de Orgão');
                ?>
            </td>
        </tr>
        <tr>
            <td class = "subtitulodireita">Nome:</td>
            <td><?php echo campo_texto('ogsdsc', 'N', 'S', 'Descrição', '56', '100', '', '', '', '', '', 'id="ogsdsc"', '', $ogsdsc); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" colspan="2" style="text-align:center">
                <input type="button" name="btnPesquisar" value="Pesquisar" id="btnPesquisar" onclick="pesquisarOrgao();">  
                <input type="button" name="btnFechar" value="Cancelar" id="btnFechar" onclick="fecharPopup();">  
            </td>
        </tr>
    </table>
</form>

<div id="div_pesquisarorgao">
<?php 
    if($_REQUEST['tpsid']) {
        $aryWhere[] = "t.tpsid = {$_REQUEST['tpsid']} ";
    }   

    $sql = "SELECT  		'<input type=\"radio\" name=\"ogsid\" id=\"ogsid_'||o.ogsid||'\" value=\"'||o.ogsid||'\" onclick=\"salvar();\">' as acao,
                    		t.tpodsc,
                    		o.ogsdsc
            FROM 			gestaodocumentos.orgaosolicitante AS o
            INNER JOIN 		gestaodocumentos.tipoorgao AS t ON t.tpoid = o.tpoid
            				".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
            ORDER BY 		o.ogsdsc";
    
    $cabecalho = array("Ação", "Tipo", "Descrição");  
    $alinhamento = Array('center', 'left', 'left');
    $tamanho = Array('3%', '30%', '40%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento); ?>
</div>

        
<script type="text/javascript">
    function fecharPopup(){
        return window.close();
    }
    
    function salvar(){
        var ogsid = document.getElementsByName('ogsid');
        
        for ( var i=0; i< ogsid.length; i++  ){ 
            if( ogsid[i].checked ){
                 var resp = ogsid[i].value;
                 continue;
            }
        } 

        if(resp > 0){
        	$('#requisicao').val('salvar');
        	$('#ogsid').val(resp);
            divCarregando();
    		jQuery.ajax({
    			type: 'POST',
    			url: 'gestaodocumentos.php?modulo=principal/popupOrgao&acao=A',
    			data: jQuery('#formulario').serialize(),
    			async: false,
    			success: function(data){
    				opener.document.getElementById("div_orgao").innerHTML = data;
    				divCarregado();
    				window.close();
    		   	}
    		});
        } else {
            alert('É necessário que selecione uma Orgão.');
        }
    }

    function pesquisarOrgao(){
    	$('#requisicao').val('pesquisarOrgao');
        divCarregando();
		jQuery.ajax({
			type: 'POST',
			url: 'gestaodocumentos.php?modulo=principal/popupOrgao&acao=A',
			data: jQuery('#formulario').serialize(),
			async: false,
			success: function(data){
				jQuery("#div_pesquisarorgao").html(data);
				divCarregado();
		   	}
		});
    }
</script>