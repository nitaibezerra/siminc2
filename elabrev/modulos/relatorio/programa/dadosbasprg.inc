<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:dadosbas.inc
   Finalidade: permitir a construção de relatorio gerencial de programas em dados básicos
    */
    // primeira parte do relatório : dados básicos
    print '<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><td><b>1. DADOS BÁSICOS</b></td></tr></table>';
     $sql= 'select p.*,o.orgdsc,tp.tprnome from programa p left join orgao o on o.orgcod=p.orgcod left join tipoprograma tp on tp.tprcod=p.tprcod where p.prgid ='.$programa;
     $RSprg = $db->record_set($sql);
     $_SESSION['rsdados'][] = $db->carrega_tudo($RSprg);
     $res = $db->carrega_registro($RSprg,0);
     if(is_array($res))
       foreach($res as $k=>$v) { ${$k}=$v; if (${$k}== '') ${$k} == 'Dado não informado pelo SIGPLAN';}

     ?>
     <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
      <tr>
        <td align='right' class="subtitulodireita" width="20%">Código:</td>
        <td><?=$prgcod?></td>
      </tr>
</form>
      <tr>
        <td align='right' class="SubTituloDireita">Orgão responsável:</td>
        <td><?=$orgdsc?></td>
      </tr>
      <?if ($unidsc){?>
      <tr>
        <td align='right' class="SubTituloDireita">Unidade responsável:</td>
         <td><?=$unidsc?></td>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Denominação:</td>
        <td><?=$prgdsc?></td>
      </tr>
      <?if ($prgdscproblema){?>
      <tr>
        <td align='right' class="SubTituloDireita">Problema:</td>
        <td><?=$prgdscproblema?></td>
      </tr>
      <?}?>
      <?if ($prgdscobjetivo){?>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo do Programa:</td>
        <td><?=$prgdscobjetivo?></td>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Público-Alvo:</td>
        <td><?=$prgdscpublicoalvo?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Justificativa:</td>
        <td><?=$prgdscjustificativa?></td>
      </tr>
      <?if ($prgdscobjetivoassoc){?>
      <tr>
        <td align='right' class="SubTituloDireita">Objetivo setorial associado:</td>
        <td><?=$prgdscobjetivoassoc?></td>
      </tr>
      <?}?>
      <?if ($mobdsc){?>
      <tr>
        <td align='right' class="SubTituloDireita">Desafio associado:</td>
        <td><?=$mobdsc?></td>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Programa estruturante?</td>
        <td>
	<?
	   //if ($prgsnestruturante) {
	     // print '<input type="radio" value="1" name="prgsnestruturante" checked>Sim &nbsp;&nbsp; <input type="radio" name="prgsnestruturante" value="0" checked>NÃ£o';
	  // }
	  // else
	  // {
	     // print '<input type="radio" value="0" name="prgsnestruturante"> Sim &nbsp;&nbsp; <input type="radio" name="prgsnestruturante" value="1"  checked>NÃ£o';
	  // }
	  print 'Dado não informado pelo SIGPLAN';
	?>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Tipo:</td>
        <td><?=$tprnome?></td>
      </tr>
      <tr>
        <td align='right' class="SubTituloDireita">Horizonte temporal:</td>
        <td>
	<?
	   if ($prgsntemporario=='f')    print 'Contínuo </td>';
	   else    print 'Temporário </td>';
	?>
      </tr>
	<?
	   if ($prgsntemporario=='t'){?>
      <tr>
        <td align='right' class="SubTituloDireita">Data Início/Término:</td>
        <td><?=$prgmesinicio?> /
	        <?=$prganoinicio?>
	<? if ($prgsntemporario=='t') {
	?>
	  <br>(Se temporário) <?=$prgmestermino?> /
      <?=$prganotermino?>
	  <?}?>
	  </td>
      </tr>

       <tr>
        <td align='right' class="SubTituloDireita">Valor Global Estimado <br>do Programa (R$) (se temporário)</td>
        <td>
	<? if ($prgsntemporario=='t') {
	?>
            <?=$prgvlrprograma?>
	  <?}?>
	  </td>
      </tr>
      <?}?>
      <tr>
        <td align='right' class="SubTituloDireita">Estratégia de implementação:</td>
        <td><?=$prgdscestrategia?></td>
    </td>
      </tr>
      <?if ($prgobs){?>
      <tr>
        <td align='right' class="SubTituloDireita">Observação:</td>
        <td><?=$prgobs?></td>
      </tr>
	  <?}?>
<?

   $sql = "select pfl.pflcod as codigo, pfl.pfldsc as descricao,pfl.pflsncumulativo,rpu.usucpf, rpu.prgid, usu.usunome || '('|| org.orgdsc ||')' as usuario from perfil pfl left join usuarioresponsabilidade rpu on rpu.pflcod = pfl.pflcod and rpu.prgid = ".$programa." and rpu.rpustatus='A' left join usuario usu on usu.usucpf=rpu.usucpf left join orgao org on org.orgcod = usu.orgcod where pfl.pflstatus='A' and pfl.pflresponsabilidade in ('P','X') order by pfl.pfldsc";
    $RS2 = $db->record_set($sql);
    $_SESSION['rsdadosresp'][] = $db->carrega_tudo($RS2);
    $nlinhas = $db->conta_linhas($RS2);
    if ($nlinhas >= 0) {
        for ($ii=0; $ii<=$nlinhas;$ii++){
            $res = $db->carrega_registro($RS2,$ii);
            // a linha abaixo transforma em variáveis todos os campos do array
            if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
	        $linha = "<tr><td align='right' class='SubTituloDireita'><b>".$descricao.":</b></td><td>".$usuario."</td></tr>";
            print $linha;
        }
     }

?>
    </table>
<?

