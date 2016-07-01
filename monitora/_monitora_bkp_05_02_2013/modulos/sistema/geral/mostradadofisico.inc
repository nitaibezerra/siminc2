<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:mostradadofisico.inc
   Finalidade: permitir a visualização dos dados fisicos de uma ação.

   */

 if ($acao2 <> $acao)
 {
 	$acao2=$acao;
   $sql = "select d.* from monitora.dadofisico d where acaid=$acao";
   
    
         	$rs2 = @$db->carregar( $sql );
         	if (  $rs2 && count($rs2) > 0 )
         	{
         		$j=0;
	             foreach ( $rs2 as $linha2 )
		          {
		          		foreach($linha2 as $k=>$v) {
		          			if ($v==0 and $v != null) ${$k}='-';
		          			else 
		          			${$k}=$v;
		          		}
		          		
		          		print '<br>'
		          		?>


    <tr>
    <TD><b> Físico</b></TD>
      <td ><?=  mostra_num($fisqtderealizado1)?></td>
      <td > <?= mostra_num($fisqtderealizado2)?></td>
      <td > <?= mostra_num($fisqtderealizado3)?></td>
      <td > <?= mostra_num($fisqtderealizado4)?></td>
      <td > <?= mostra_num($fisqtderealizado5)?></td>
      <td > <?= mostra_num($fisqtderealizado6)?></td>
      <td > <?= mostra_num($fisqtderealizado7)?></td>
      <td > <?= mostra_num($fisqtderealizado8)?></td>
      <td > <?= mostra_num($fisqtderealizado9)?></td>
      <td > <?= mostra_num($fisqtderealizado10)?></td>
      <td > <?= mostra_num($fisqtderealizado11)?></td>
      <td > <?= mostra_num($fisqtderealizado12)?></td>
      <td > <?= mostra_num($fisqtdeprevistoano)?></td>
      <td > <?= mostra_num($fisqtderealizadoano)?></td>
     <td ><?
           if ($fisqtdeprevistoano) print number_format(intval($fisqtderealizadoano*100/$fisqtdeprevistoano),0)?></td>      
    </tr>

 <? if (trim($fisdsccomentexecucao)<>'') {?> 
 <tr><td colspan="16">
<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr><td><textarea rows="5" name="S1" cols="83"><?=$fisdsccomentexecucao?></textarea></td></tr>
</table>
</tr></td>


<?}?>



		          		<?
		          		$j++;
		          }
         		
         	}
         	
 }