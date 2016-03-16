 <html>
  <head>
 
    <title><?php echo $titulo ?></title> 
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
    </style>
  </head>
<br>    
<?
				$sql = "SELECT
                			coiid, 
                			coidsc,
                			coiitem,
                			coiespecificacao, 
                			coiqtde
                		FROM 	
                			evento.coitem 
                		WHERE 
                			coiid = {$_REQUEST['id']}
                ";
				if( $_REQUEST['id'] ) 
                	$rsDadosItem = $db->carregar( $sql );
?>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
<tr>
	<td width="100%" align="center"><label class="TituloTela" style="color:#000000;">
		Detalhamento de especificações</label>
	</td>
	</tr>
	<tr>
	<td width="100%" align="center"><label class="TituloTela" style="color:#000000;">
		item: <?= $rsDadosItem[0]['coidsc'];  ?></label>
	</td>
	</tr>
	<tr> 
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
          
             
            <tr>
                <td class ="SubTituloDireita" align="right">Especificação dos ítens: </td>
                <td>  
                <?php
                	echo $rsDadosItem[0]['coiespecificacao'];
                ?>
            	</td>
                <td align="left">   
                </td>
             </tr> 
       </table>
       <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
			<tr>
				<td class="SubTituloDireita" colspan="2" style="text-align:center">
					 <input type="button" name="btnFechar" value="Fechar" id="btnFechar" onclick="fecharPopup();">  
				</td>
			</tr>
	</table>
 <script type="text/javascript">
 function fecharPopup(){
			return window.close();
		}
 </script>