<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   Módulo:mostradadofinanceiro.inc
   Finalidade: permitir a visualização dos dados financeiros de uma ação.
   */
         	$sql = "select d.*,f.fppdsc from dadofinanceiro d inner join fonteppa f on f.fppcod=d.fppcod where acaid=$acao";

         	$rs2 = @$db->carregar( $sql );
         	if (  $rs2 && count($rs2) > 0 )
         	{
         		$j=0;
	             foreach ( $rs2 as $linha2 )
		          {
		          		foreach($linha2 as $k=>$v) {
		          			if ($v==0 and $v != null) ${$k}='-';
		          			else 
		          			${$k}=$v;}		          				          				          
		          		?>
		          		
    <tr>
    <TD><b> Financeiro</b></TD>
      <td > <?= mostra_num($finvlrrealizado1,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado2,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado3,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado4,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado5,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado6,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado7,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado8,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado9,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado10,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado11,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizado12,2,'R$')?></td>
      <td > <?= mostra_num($finvlrprevistoano,2,'R$')?></td>
      <td > <?= mostra_num($finvlrrealizadoano,2,'R$')?></td>
     <td ><?
           if ($finvlrprevistoano) print number_format(intval($finvlrrealizadoano*100/$finvlrprevistoano),0)?></td>      
    </tr>
 <? if (trim($findsccomentexecucao)<>'') {?>
    <tr><td colspan="16">
 
<table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

<tr><td><textarea rows="5" name="S1" cols="83"><?=$findsccomentexecucao?></textarea></td></tr>
</table>
</td></tr>
<?}
if ($finvlrprevistoano1 or $finvlrprevistoano2 or $finvlrprevistoano3 or $finvlrprevistoano4 or $finvlrprevistoano5 or $finvlrprevistoano6) {
?>
    <tr><td colspan="16">
    <table border="0" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr><td><b>Previsão ao longo do PPA</b></td></tr>
<tr><td>
  <table border="1" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="100%" id="AutoNumber2">
    <tr>
      <td ><b><?=$ano1?></b></td>
      <td ><b><?=$ano2?></b></td>
      <td ><b><?=$ano3?></b></td>
      <td ><b><?=$ano4?></b></td>
      <td ><b><?=$ano5?></b></td>
      <td ><b><?=$ano6?></b></td>
    </tr>
    <tr>
      <td > <?= mostra_num($finvlrprevistoano1,2,'R$')?></td>
      <td > <?= mostra_num($finvlrprevistoano2,2,'R$')?></td>
      <td > <?= mostra_num($finvlrprevistoano3,2,'R$')?></td>
      <td > <?= mostra_num($finvlrprevistoano4,2,'R$')?></td>
      <td > <?= mostra_num($finvlrprevistoano5,2,'R$')?></td>
      <td > <?= mostra_num($finvlrprevistoano6,2,'R$')?></td>
    </tr>
  </table>
</td></tr>
</table>
</td></tr>
<?}?>


		          		<?
		          		$j++;
		          }
         		
         	}