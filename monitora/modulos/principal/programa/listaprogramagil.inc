<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral, Gilberto Arruda Cerqueira Xavier
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com), Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:listaprograma.inc
   Finalidade: permitir o controle da lista de programas
   */
//Recupera tudo que foi passado por REQUEST e instancia as variáveis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;
//Agrupamento da listagem
if (!$ordemgrupo) $ordemgrupo='A';
if ($ordemgrupo=='L') $agrupadopor = 'Localizador'; elseif ($ordemgrupo=='U') $agrupadopor = 'Unidade'; else $agrupadopor = 'Ação';
//Ordem da listagem
if (!$ordemlista) $ordemlista = 2;
//Direcionamento da ordenação ASC / DESC
if ($ordemlistadir <> 'DESC') {$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';} else $ordemlistadir2 = 'ASC';
//Controla se vai aparecer os programas baseado na ação ou no parâmetro
if (!$listaprg and $acao=='A') $listaprg='N'; elseif (!$listaprg) $listaprg='S';

//Se não tiver escolhido nehum tipo traz todas
if (!$tipoacao1 and !$tipoacao2 and !$tipoacao3) {$tipoacao1='N';$tipoacao2='R';$tipoacao3='E';}
//filtros de tipo de ação
if ($tipoacao1 and !$tipoacao2 and !$tipoacao3) $wh .= " and a.acasnrap='f' and a.acasnemenda='f' ";
elseif ($tipoacao1 and $tipoacao2 and !$tipoacao3) $wh .= " and a.acasnemenda='f' ";
elseif ($tipoacao1 and !$tipoacao2 and $tipoacao3) $wh .= " and a.acasnrap='f'";
elseif (!$tipoacao1 and $tipoacao2 and $tipoacao3) $wh .= " and a.acasnrap='t' and a.acasnemenda='t' ";
elseif (!$tipoacao1 and $tipoacao2 and !$tipoacao3) $wh .= " and a.acasnrap='t' ";
elseif (!$tipoacao1 and !$tipoacao2 and $tipoacao3) $wh .= " and a.acasnemenda='t' ";
else $wh .= "";

//Cabeçalho do sistema
include  APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?

if ($listaprg=='N') {
		if ($ordemgrupo=='A') {
			$subtit1 = 'de Ações'; 
			$subtit2 = 'da Ação'; 
			$sql = "select a.prgcod, a.acacod as codigo, a.acadsc as descricao, a.prgano, count(*) as numacoes from acao a where a.acastatus='A'  and a.prgano = '".$_SESSION['exercicio']."' ".$wh." group by a.prgcod, a.acacod , a.acadsc, a.prgano order by ".$ordemlista." ".$ordemlistadir;
			}
		elseif ($ordemgrupo=='U') {
			$subtit1 = 'de Unidades'; 
			$subtit2 = 'da Unidade'; 
			$sql = "select a.unicod, a.unicod as codigo, u.unidsc as descricao, a.prgano, count(*) as numacoes from acao a inner join unidade u on a.unicod=u.unicod where a.acastatus='A'  and a.prgano = '".$_SESSION['exercicio']."'  ".$wh." group by a.unicod, a.unicod , u.unidsc, a.prgano order by ".$ordemlista." ".$ordemlistadir;
			}
		elseif ($ordemgrupo=='L') {
			$subtit1 = 'de Localizadores'; 
			$subtit2 = 'do Localizador';
			$sql = "select a.regcod, a.regcod as codigo, r.regdsc as descricao, a.prgano, count(*) as numacoes from acao a left join regiao r on a.regcod=r.regcod where a.acastatus='A'  and a.prgano = '".$_SESSION['exercicio']."'  ".$wh." group by a.regcod, a.regcod , r.regdsc, a.prgano order by ".$ordemlista." ".$ordemlistadir;
			}
	}
else {
	$tipoprgaca = "Programas";

	$sql = "select p.prgid, p.prgcod as codigo, prgdsc as descricao, p.prgano, count(*) as numacoes from programa p inner join acao a on p.prgid=a.prgid where p.prgstatus='A'  and p.prgano = '".$_SESSION['exercicio']."'  ".$wh." group by p.prgid, p.prgcod , p.prgdsc, p.prgano order by ".$ordemlista." ".$ordemlistadir;
	$subtit1 = 'de Programas'; 
	$subtit2 = 'do Programa';
	}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Consultar Programas e Ações','Clique no código para ver detalhes - Total '.$subtit1.' ('.($nlinhas+1).')');?>
<table width="95%" border="0" cellspacing="0" cellpadding="2" align="center" bgcolor="#f7f7f7" style="border-top: 1px solid #c0c0c0;">
    <form name="formulario" method="post">
	<input type="Hidden" name="ordemlista" value="<?=$ordemlista?>">
	<input type="Hidden" name="ordemlistadir" value="<?=$ordemlistadir?>">
	<tr>
	<td class="SubTituloDireita">Tipo de Ação:</td>
	<td style="color:#000080;">
	   <input type="checkbox" name="tipoacao1" value="N" <?if ($tipoacao1) print 'checked';?>> Normal 
	   <input type="checkbox" name="tipoacao2" value="R" <?if ($tipoacao2) print 'checked';?>> RAP 
	   <input type="checkbox" name="tipoacao3" value="E" <?if ($tipoacao3) print 'checked';?>> Emenda
	</td>
	<td class="SubTituloDireita">Mostrar:</td><td style="color:#008000;"><input type="radio" name="listaprg" value="S"  <?if ($listaprg=='S') print 'checked';?>> Programas e Ações <input type="radio" name="listaprg" value="N"  <?if ($listaprg=='N') print 'checked';?>> Apenas Ações  </td>
	<td class="SubTituloDireita">Agrupado por:</td><td style="color:#800000;"><input type="radio" name="ordemgrupo" value="A" <?if ($ordemgrupo=='A') print 'checked';?>> Ação <input type="radio" name="ordemgrupo" value="U" <?if ($ordemgrupo=='U') print 'checked';?>> Unidade <input type="radio" name="ordemgrupo" value="L" <?if ($ordemgrupo=='L') print 'checked';?>> Localizador</td>
	<td class="SubTituloDireita" style="text-align:center;"><input type="Button" style="width:80px;" name="Filtrar" value="Filtrar" onclick="filtra();"></td>
	</tr>
	</form>
</table>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
    <tr>
      <td valign="top" class="title"  width="55"><?if ($ordemlista == '2') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('2','<?=$ordemlistadirnova?>');" title="Ordenar por Código"><strong>Código</strong></label></td>
      <td valign="top" class="title"><?if ($ordemlista == '3') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('3','<?=$ordemlistadirnova?>');" title="Ordenar por Título <?=$subtit2?>"><strong>Título <?=$subtit2?></strong></label></td>
      <td valign="top" class="title" width="45"><?if ($ordemlista == '5') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('5','<?=$ordemlistadirnova?>');"  title="Ordenar por Total"><strong>Total</strong></label></td>
    </tr>
</thead>
<tbody>
<?
$totalacoes = 0;
for ($i=0; $i<=$nlinhas;$i++){
  $res = $db->carrega_registro($RS,$i);
// a linha abaixo transforma em variáveis todos os campos do array
  if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
     $totalacoes = $totalacoes + $numacoes;
     if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
?>
     <tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';" >
		 <td><?
		 if ($listaprg=='S'){?><a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/cadprograma&acao=C&prgid=<?=$prgid?>"><?=$codigo?></a><?}else{if ($ordemgrupo=='A') print $prgcod.'.'.$codigo; else print $codigo;}?></td>
		 <?if ($descricao=='') $descricao = '<font color="red">Não atribuído pelo SIGPLAN</font>'?>
		 <td onclick="abreconteudo(<?if ($listaprg=='S') {?>'geral/listaacao.php?prgid=<?=$prgid?>&ordem=<?=$ordemgrupo?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>'<?} else {?>'geral/listaacao.php?codigo=<?if ($ordemgrupo=='A') print $prgcod.'.'.$codigo; else print $codigo;?>&ordem=<?=$ordemgrupo?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>'<?}?>,'<?=$i?>-<?=$codigo?>')" <?if ($listaprg=='N') print 'style="color:#003c7b"';?>> <img src="../imagens/mais.gif" name="+" border="0" id="img<?=$i?>-<?=$codigo?>"> <?=$descricao?></td>
	     <td align=right>(<?=$numacoes?>)</td>
	 </tr>
	 <tr bgcolor="<?=$marcado?>">
		 <td></td>
		 <td id="td<?=$i?>-<?=$codigo?>"></td><td></td>
	 </tr>
<?}
if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';?>
    <tr class="<?=$marcado?>">
	<td></td>
      <td valign="top" class="title"> 
	  <table width="100%"><tr><td><strong>Total <?=$subtit1?>: <?=$i?></strong></td>
	  <td align="right"><strong>Total de Localizadores de Gasto:</strong></td></tr></table>
	  </td>
	  <td align="right"><strong><?=$totalacoes?></strong></td>
   </tr>
</tbody>
</table>

      </td>
    </tr>
  </table>
  </center>
</div>
<script language="JavaScript">
function filtra()
{
document.formulario.submit();
}

function ordena(ordem, direcao)
{
document.formulario.ordemlista.value=ordem;
document.formulario.ordemlistadir.value=direcao;
document.formulario.submit();
}
</script>
