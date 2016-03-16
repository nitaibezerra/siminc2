<?php 
    header('content-type: text/html; charset=ISO-8859-1');

    if( is_numeric($_POST['mntid']) && $_REQUEST['acaoForm'] == 'salvar' ){
        $sql = "
            SELECT  mntid,
                    mntcnpj, 
                    mntdsc
            FROM gestaodocumentos.mantenedoras
            WHERE mntid = ".$_POST['mntid'];
        $rs  = $db->carregar( $sql );

        if( $rs ){
            $_SESSION['iescodSessionEntidade'] = $_POST['mntid'];
            $resp = "<span style=\"color: #1874CD;\"\">".$rs[0]['mntid']." - ".$rs[0]['mntcnpj']." - ".$rs[0]['mntdsc']."</a>";
            $resp .= "<input type=\"hidden\" name=\"mntid\" id=\"mntid\" value=\"{$rs[0]['mntid']}\" />";
            die( $resp );
        }
    }
?>
<html>
    <head>
        <script type="text/javascript" src="/includes/prototype.js"></script>
        <script type="text/javascript" src="/includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    </head>
    <br>    
    <form name="formulario" id="formulario" method="POST">
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <input type="hidden" name="acaoForm" id="acaoForm" value="1" />
            <input type="hidden" name="tarid" id="tarid" value="" />
            <tr>
                <td class = "subtitulodireita" colspan="2" style="text-align: center; font-size: 12px;"><h3>Busca de instituição</h3></td> 
            </tr>
            <tr>
                <td class = "subtitulodireita" width="25%">CNPJ:</td>
                <td>
                    <?= campo_texto('mntcnpj', 'N', 'S', 'CNPJ', 57, 20, '###################', '', '', '', '', 'id="mntcnpj"', '', ''); ?>
                </td>
            </tr>
            <tr id="tr_nome">
                <td class = "subtitulodireita">Nome:</td>
                <td>
                    <?= campo_texto('mntdsc', 'N', 'S', 'Descrição da Mantenedota', 57, 50, '', '', '', '', '', 'id="mntdsc"', '', ''); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="btnPesquisar" value="Pesquisar" id="btnPesquisar" onclick="buscar();">  
                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvar();">  
                    <input type="button" name="btnFechar" value="Cancelar" id="btnFechar" onclick="fecharPopup();">  
                </td>
            </tr>
    </form>
	<?PHP 
        if( $_POST['mntcnpj'] != '' ) {
            $mntcnpj = $_POST['mntcnpj'];
            $AND .= " AND mntcnpj ILIKE ('%{$mntcnpj}%') "; 
        }
        if( $_POST['mntdsc'] != '' ) {
            $mntdsc = removeAcentos($_POST['mntdsc']);
            $AND .= " AND public.removeacento(mntdsc) ILIKE ('%{$mntdsc}%') "; 
        }
        
        $acao = "<input type=\"radio\" name=\"mntid\" id=\"mntid_'||mntid||'\" value=\"'||mntid||'\" onclick=\"salvar();\" > &nbsp;";
        $sql = "
            SELECT  '{$acao}' as acao,
                    CASE WHEN mntcnpj <> ''
                        THEN 
                            CASE WHEN CHAR_LENGTH(mntcnpj) = 11 
                                THEN trim( replace( to_char( cast(mntcnpj as bigint), '000:000:000-00' ), ':', '.' ) )
                                ELSE trim( replace( to_char( cast(mntcnpj as bigint), '00:000:000/0000-00' ), ':', '.' ) )
                            END 
                        ELSE ''
                    END AS mntcnpj,
                    mntdsc
            FROM gestaodocumentos.mantenedoras
            WHERE 1=1 $AND 
            ORDER BY 3 
        ";
        $cabecalho = array( "&nbsp","CNPJ","Descrição da Mantenedora"  );  
        $alinhamento = Array('center', 'left', 'left' );
        $tamanho = Array( '5%','10%', '50%' );
        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
	?>
</table>
        
<script type="text/javascript">
    
    function fecharPopup(){
        return window.close();
    }
    
    function buscar(){
        document.formulario.acaoForm.value = 'busca'; 
        document.formulario.submit();
    }
    
    function cancelar(){
        document.formulario.acaoForm.value = 'cancelar'; 
        document.formulario.submit();
    }
    
    function salvar(){
        var mntid = document.getElementsByName('mntid');

        for ( var i=0; i< mntid.length; i++  ){ 
            if( mntid[i].checked ){
                 var resp = mntid[i].value;
                 continue;
            } 
        }
        if( resp > 0 ){
            var req = new Ajax.Request(
                'gestaodocumentos.php?modulo=principal/popupMantenedora&acao=A&acaoForm=salvar', {
                method:     'POST',
                parameters: '&mntid=' + resp,
                onComplete: function (res){
                        opener.document.getElementById("div_mantenedora").innerHTML = res.responseText;		  
                        window.close();
                    }
            });
        }else{
            alert('É necessário que selecione uma Mantenedora.');
        }
    }
</script>