<?php 
    header('content-type: text/html; charset=ISO-8859-1');
    
    if( $_POST['ajaxestuf'] ){	
        $sql = "
            SELECT  muncod as codigo, 
                    mundescricao as descricao 
            FROM territorios.municipio
            WHERE estuf = '{$_POST['ajaxestuf']}' 
            ORDER BY mundescricao asc
        ";
        echo $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', '420', 'S', 'muncod', false, null, 'Município');
        die();
    } 

    if( is_numeric($_POST['iesid']) && $_REQUEST['acaoForm'] == 'salvar' ){
        $sql = "
            SELECT  iesid,
                    iessigla, 
                    iesdsc
            FROM gestaodocumentos.instituicaoensino 
            WHERE iessituacao = 'A' AND iesid = ".$_POST['iesid'];
        $rs  = $db->carregar( $sql );

        if( $rs ){
            $_SESSION['iescodSessionEntidade'] = $_POST['iesid'];
            $resp = "<span style=\"color: #1874CD;\"\">".$rs[0]['iesid']." - ".$rs[0]['iessigla']." - ".$rs[0]['iesdsc']."</span>";
            $resp .= "<input type=\"hidden\" name=\"iesid\" id=\"iesid\" value=\"{$rs[0]['iesid']}\" />";
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
                <td class = "subtitulodireita" width="25%">Código:</td>
                <td>
                    <?= campo_texto('iesid', 'N', 'S', 'iesid', 57, 20, '###################', '', '', '', '', 'id="iesid"', '', ''); ?>
                </td>
            </tr>
            <tr id="tr_nome">
                <td class = "subtitulodireita">Nome:</td>
                <td>
                    <?= campo_texto('iesdsc', 'N', 'S', 'Nome', 57, 50, '', '', '', '', '', 'id="iesdsc"', '', ''); ?>
                </td>
            </tr>
            <tr id="tr_sigla">
                <td class = "subtitulodireita">Sigla:</td>
                <td>
                    <?= campo_texto('iessigla', 'N', 'S', 'Nome', 57, 20, '', '', '', '', '', 'id="iessigla"', '', ''); ?>
                </td>
            </tr>
            <tr id="tr_mantenedora">
                <td class = "subtitulodireita">Mantenedora:</td>
                <td colspan="2">
                    <?PHP
                        $sql = "
                            SELECT  mntid AS codigo,
                                    mntdsc as descricao
                                    --mntcnpj||' - '||mntdsc AS descricao
                            FROM gestaodocumentos.mantenedoras
                            WHERE 1=1
                        ";
                        $arCampos = array(
                            array(
                                "codigo" => "mntcnpj",
                                "descricao" => "<b>CNPJ:</b>",
                                "tipo" => "1"),
                            array(
                                "codigo" => "mntdsc",
                                "descricao" => "<b>Descrição:</b>",
                                "tipo" => "0")
                         );                   
                        $ogcid = array(
                                "value" => $dadosServ['mntid'],
                                "descricao" => $dadosServ['mntdsc']
                        );
                        campo_popup('mntid', $sql, 'Selecione a Unidade Mantenedora', null, '400x800', '56', $arCampos, 1, false, $habilitado['fal'], false, 'N');                                                                                
                    ?>                

                </td>
            </tr>
            <tr id="tr_municipio">
                <td class = "subtitulodireita">Município:</td>
                <td>
                    <?= campo_texto('mundsc', 'N', 'S', 'Municípo', 57, 100, '', '', '', '', '', 'id="mundsc"', '', ''); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="btnPesquisar" value="Pesquisar" id="btnPesquisar" onclick="buscar();">  
                    <!--input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvar();"-->  
                    <input type="button" name="btnFechar" value="Cancelar" id="btnFechar" onclick="fecharPopup();">  
                </td>
            </tr>
    </form>
	<?PHP 
        if( $_POST['iesid'] != '' ){ 
            $AND .= " AND i.iesid = {$_POST['iesid']}";
        }
        if( $_POST['iesdsc'] != '' ) {
            $iesnome = $_POST['iesdsc'];
            $AND .= " AND public.removeacento(i.iesdsc) ILIKE ('%{$iesnome}%') "; 
        }
        if( $_POST['iessigla'] != '' ) {
            $iessigla = removeAcentos($_POST['iessigla']);
            $AND .= " AND public.removeacento(i.iessigla) ILIKE ('%{$iessigla}%') "; 
        }    
        if( $_POST['mntid'] != '' ) {
            $AND .= " AND i.mntid = {$_POST['mntid']}"; 
        }
        if( $_POST['mundsc'] != ''){
            $mundsc = removeAcentos($_POST['mundsc']);
            $AND .= " AND public.removeacento(i.iesmunicipio) ILIKE ('%{$mundsc}%')"; 
        }
        
        $acao = "<input type=\"radio\" name=\"iesid\" id=\"iesid_'||iesid||'\" value=\"'||iesid||'\" onclick=\"salvar();\"> &nbsp;";
        $sql = "
            SELECT  '{$acao}' as acao,
                    ' '||i.iesid,
                    i.iessigla as sigla ,
                    i.iesdsc  as nome,
                    m.mntdsc,
                    i.iesmunicipio as muncod
            FROM gestaodocumentos.instituicaoensino AS i  
            LEFT JOIN gestaodocumentos.mantenedoras AS m ON m.mntid = i.mntid
            WHERE i.iessituacao = 'A' $AND 
            ORDER BY 4,3,6 
        ";
        $cabecalho = array( "&nbsp", "Código", "Sigla", "Nome", "Mantenedora", "Cidade"  );  
        $alinhamento = Array('center', 'left', 'left', 'left', 'left', 'left');
        $tamanho = Array('4%', '3%', '4%', '45%', '40%', '4%');
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
        var iesid = document.getElementsByName('iesid');

        for ( var i=0; i< iesid.length; i++  ){ 
            if( iesid[i].checked ){
                 var resp = iesid[i].value;
                 continue;
            } 
        }
        if( resp > 0 ){
            var req = new Ajax.Request(
                'gestaodocumentos.php?modulo=principal/popupInstituicoes&acao=A&acaoForm=salvar', {
                method:     'POST',
                parameters: '&iesid=' + resp,
                onComplete: function (res){
                        opener.document.getElementById("div_instituicao").innerHTML = res.responseText;		  
                        window.close();
                    }
            });
        }else{
            alert('É necessário que selecione uma Instituição.');
        }
    }
</script>