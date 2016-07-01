<?php include APPRAIZ . "includes/cabecalho.inc"; ?>
<br />
<?php
monta_titulo( 'Formulário de Impressão', '' );
?>

<form method="POST" name="formulario" id="formulario" target="blank" action="http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/reuni.php?modulo=principal/popup_monitoramento&acao=C">
<input
	type="hidden" id="exportar_xls" name="exportar_xls" value="false"> <input
	type="hidden" id="enviado" name="enviado">
	<input type="hidden" id="rel_consolidado" name="rel_consolidado" value="false">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td align='right' width='80' class="SubTituloDireita">Unidade</td>
		<td width='550'><?
		$sql = " SELECT
                                u.unicod, 
                                u.unidsc 
                            from 
                                public.unidade  as u
                            where  
                                u.unistatus = 'A' and  u.unitpocod = 'U' and u.gunid = 3
                            ORDER BY  u.unidsc";

		$resultado = $db->carregar($sql);
		?> <select id='unicod' size='6' multiple name='unicod[]'>
		<?
		if(is_array($resultado))
		{
			for($i=0;  $i  < count($resultado); $i++)
			{
				echo "<option ";
				for( $n = 0; $n < count( $_REQUEST['unicod'] );  $n++){
					if($_REQUEST['unicod'][$n] == $resultado[$i]['unicod']){
						echo " selected='selected' ";
					}
				}
				echo " value='".$resultado[$i]['unicod']."'>".$resultado[$i]['unicod'].'  - '.$resultado[$i]['unidsc']."</option>";
			}

		}
		?>
		</select> 
		<br/>
		<input type="checkbox" name="pilpas" id="pilpas"
			onclick="selecionarTodas();" /> <label for='pilpas'>
			Selecionar Tudo</label>
		</td>

	</tr>
	<tr bgcolor="#C0C0C0">
		<td colspan="2" valign="top" align="center">		
		
		<input type='button' class="botao"  name='consultar' id="consultar"
			value='consultar' onclick="enviar();" />
		</td>
	</tr>	
</table>
</form>

<script type="text/javascript">

    function enviar ()
    {
	  
	  var unicod = document.getElementById('unicod');	
	  document.getElementById('enviado').value = true;  	  
	 
	    
	  if (unicod.selectedIndex =='-1') 
	  {
	    alert('Selecione ao menos uma Unidade');
	    return false;
	  } 
	  relatorioMonitoramento(); 
	  document.formulario.submit();  
	  //relatorioMonitoramento();
    }

    
    function selecionarTodas ()
    {
		
		var pilpas = document.getElementById('pilpas');
		
		
	        var tudo = document.getElementById('unicod');
	
	        if(pilpas.checked == true )
	        {
				for (var i = 0; i< tudo.options.length; i++) 
				{
				   tudo.options[i].selected = true;
				}
	        }
	        else
	        {
				for (var i = 0; i< tudo.options.length; i++) 
				{
				   tudo.options[i].selected = false;
				}
	        }
    }
    
   function relatorioMonitoramento()
	{
		window.open( 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/reuni/reuni.php?modulo=principal/popup_monitoramento&acao=C','blank','height=800,width=800,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no');
	}
</script>