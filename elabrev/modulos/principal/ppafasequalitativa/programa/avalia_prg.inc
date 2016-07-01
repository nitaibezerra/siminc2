 <?
  /*
   sistema simec
   setor responsável: spo-mec
   desenvolvedor: equipe consultores simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com)
   módulo:avalia_prg.inc
   finalidade: permitir a avaliação de programa
   */
   if ($refdata_limite_avaliacao_prg >= date('Y-m-d')) $habil = 'S';
   else {
     // não está autorizado na data
     // então testa se tem alguma autorização especial
     $sql= "select ae.aelid from autorizacaoespecial ae where ae.prgid =".$_SESSION['prgid']." and ae.aelstatus ='A' and ae.aeldata_inicio <='".date('Y-m-d')."' and ae.aeldata_fim >='".date('Y-m-d')."' and ae.togcod=1 and ae.refcod=".$refcod;
    $registro=$db->recuperar($sql);
    if (is_array($registro)) $habil = 'S'; else
       $habil = 'N';
   }
   // a variavel HABIL indica se esta dentro da data
   $mostra_av = true;
   //testo se não for o titular, então, verifico se está liberado para o público
   if (!$autorizado_av and ! $autorizado_dig)
   {
      if (! $avlib or $avlib=='f') {$avaliacao = '<h2><center>Não liberada</center></h2>';$mostra_av = false;}
   }
   else
   	{
         if ((! $avlib or $avlib=='f') and $habil == 'N')
         {
           $avaliacao = '<h2><center>Prazo para liberação expirado</center></h2>';
           $mostra_av = false;}
    }

   if (! $avaid) $avaid=0;
   $aval ='avaliacao'.'_'.$refcod.'_'.$avaid.'_'.$i;
   ${$aval} = $avaliacao;


  ?>
  <table border="0" cellspacing="1" cellpadding="3" align="center" bgcolor="#F5F5F5" style="width:100%;color:#808080;">

<?
   if ((! $autorizado_av and ! $autorizado_dig) or $habil=='N' or $avlib=='t')
   {
    ?>
       <tr>
          <td colspan='4'><DIV class="treeview" style="OVERFLOW:AUTO; WIDTH:100%; HEIGHT:173px; BORDER:2px SOLID #ECECEC; background-color: White;"><? if ($mostra_av==true) {?><div align="right"><img src="imagens/preview.gif" border="0" align="absmiddle"> <a href="javascript:visualiza('<?=md5_encrypt($avaid,'')?>')">Ver em tela cheia.</a></div><?}?><?=$avaliacao ?></div>
		  </td></tr>
  <? }
     else
     {
       // então é o titular ou um digitador e pode digitar
       ?>
	   <tr>
	      <td colspan="2" align="center"><?=campo_textarea("$aval",'N',$habil,'',65,13,'')?></td></tr>
     <?}?>
   <?
     // segundo nível da tabela, caso já tenha sido liberada, apresenta os dados de data
     // situação e cor
     if ($avlib=='t')
     {
       ?>
       <tr><td align="right" class="subtitulodireita">Situação:</td><td><img src="imagens/<?=trim(strtolower($corimgav))?>" border="0" title="<?=trim($avcordsc)?>"> <?=$avtpsdsc?></td><td align="right" class="subtitulodireita">Data:</td><td><?=$avdata?></td></tr>
       <?
	    $sql="select u.usunome,u.usuemail,u.usufoneddd,u.usufonenum,o.orgdsc from usuario u left join orgao o on u.orgcod = o.orgcod where u.usucpf='".$avusu."'";
        $RSu = $db->record_set($sql);
        $resu =  $db->carrega_registro($RSu,0);
        if(is_array($resu)) foreach($resu as $k=>$v) ${$k}=$v;
        print '<tr><td align="right" class="subtitulodireita">Resp.:</td><td>';
		if ($avusu<>$_SESSION['usucpf']) 
		{ // não manda e-mail para ele mesmo
		print '<img src="imagens/email.gif" title="Envia e-mail ao Gestor" border="0" onclick="envia_email(\''.$avusu.'\');"> ';
		}
		print $usunome.'</td><td align="right" class="subtitulodireita">Tel.:</td><td>'.$usufonenum.'</td></tr>';
        print '<tr><td align="right" class="subtitulodireita">Órgão:</td><td>'.$orgdsc.'</td></tr>';
     }
     else
     {
       // apresenta a situação e a escolha de cor
       if ($mostra_av==true){
       print '<tr><td align="right" class="subtitulodireita">Situação:</td><td>';
	       // pode fazer avaliação
	      $sql = "select tpscod as CODIGO,tpsdsc as descricao from tiposituacao where tpsstatus='A' order by tpsdsc ";
		  ${"tpscodav".$i}=$avtps;
		  $db->monta_combo("tpscodav".$i,$sql,'S',"Escolha uma situação",'','');
		  print '</td></tr>';
	      print '<tr><td align="right" class="subtitulodireita">Cor:</td><td>';
		  $sql = "select corcod as CODIGO,corsignificado as desc , corimgav as imagem from cor where corstatus='A' order by corcod ";
		  $RSradio = $db->record_set($sql);
	      $nlinhasradio = $db->conta_linhas($RSradio);
	      for ($j=0; $j<=$nlinhasradio;$j++)
	      {
	        $res2 =  $db->carrega_registro($RSradio,$j);
	        if(is_array($res2)) foreach($res2 as $k=>$v) ${$k}=$v;
	        if ($codigo == $avcor) $ck = ' checked '; else $ck ='';
	        print "<input type='radio' value=".$codigo.$ck." name='corcodav".$i."'  title='".$desc."'>";
	        print "<img src='imagens/".trim($imagem)."' title='".$desc."'>";
          }
          print "</td></tr>";
          }
      }

    // quarto nível da tabela, botões de ação
    // haverá três tipos de botão: gravar, liberar, bloquear
    // se for digitador ou titular e habil = s e não liberado, mostra o botão gravar
    if (($autorizado_av or $autorizado_dig) and $habil=='S')
    {
      print '<tr><td align="right" class="subtitulodireita">Ações:</td><td colspan="4">';
      if (! $avlib or $avlib=='f' or $avaid==0)
      {
         print '<input type="button" class="botao" name="btgrava" value= "Gravar" onclick="grava_av('.$refcod.','.$avaid.','.$i.')">&nbsp;&nbsp;&nbsp;';
      }
      // se for o titular e  ainda está dentro do prazo, pode liberar ou bloquear para nova edição
      if ($autorizado_av)
      {
         if ((!$avlib or $avlib=='f') and $avaid > 0)
         {
            print '&nbsp;&nbsp;&nbsp;<input type="button" class="botao" name="btlibera" value= "Liberar" onclick="libera_av('.$refcod.','.$avaid.','.$i.')">&nbsp;&nbsp;&nbsp;';
         }
         else if ($avaid > 0)
         {
            print '&nbsp;&nbsp;&nbsp;<input type="button" class="botao" name="btlibera" value= "Bloquear avaliação liberada" onclick="bloquea_av('.$refcod.','.$avaid.','.$i.')">&nbsp;&nbsp;&nbsp;';
         }
      }
    }
    print '</td></tr>';
   ?>
   </table>
