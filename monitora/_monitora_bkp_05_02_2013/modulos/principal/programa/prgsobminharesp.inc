<?
//Recupera tudo que foi passado por REQUEST e instancia as variáveis correspondentes
foreach($_REQUEST as $k=>$v) ${$k}=$v;
//Agrupamento da listagem
if (!$ordemgrupo) $ordemgrupo='A';
if ($ordemgrupo=='L') $agrupadopor = 'Localizador'; elseif ($ordemgrupo=='U') $agrupadopor = 'Unidade'; else $agrupadopor = 'Ação';
//Ordem da listagem
if (!$ordemlista) $ordemlista = 2;
//Direcionamento da ordenação ASC / DESC
if ($ordemlistadir <> 'DESC') {$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';} else $ordemlistadir2 = 'ASC';
//Cabeçalho do sistema
include APPRAIZ."includes/cabecalho.inc";
?>
<br>
<?
$sql = "select p.prgid, p.prgcod as codigo, prgdsc as descricao, p.prgano, count(*) as numacoes from programa p inner join acao a on p.prgid=a.prgid inner join usuarioresponsabilidade u on p.prgid=u.prgid where p.prgstatus='A'  and p.prgano = '".$_SESSION['exercicio']."'  and  u.usucpf='".$_SESSION['usucpf']."' and u.rpustatus='A' ".$wh." group by p.prgid, p.prgcod , p.prgdsc, p.prgano order by ".$ordemlista." ".$ordemlistadir;
$subtit1 = 'de Programas'; 
$subtit2 = 'do Programa';
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);

//$sql = "select prgid from usuarioresponsabilidade where u.usucpf='".$_SESSION['usucpf']."' and u.rpustatus='A' and prgid is not null";
//$RS=$db->record_set($sql);
$nlinhas = $db -> conta_linhas($RS);
if ($nlinhas>=0)
	{
	for ($i=0;$i<=$nlinhas;$i++)
		{
		$res = $db -> carrega_registro($RS,$i);			
		$prgid_r .= $res[0]."|,|";
		}
	}
if ($prgid_r) $prgid_r = substr($prgid_r,0,strlen($prgid_r)-3);
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Programas Sob Minha Responsabilidade','Clique para ver detalhes.');?>

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
 <form name="formulario" method="post">
	<input type="Hidden" name="ordemlista" value="<?=$ordemlista?>">
	<input type="Hidden" name="ordemlistadir" value="<?=$ordemlistadir?>">
<thead>
    <tr>
      <td valign="top" class="title"  width="55"><?if ($ordemlista == '2') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('2','<?=$ordemlistadirnova?>');" title="Ordenar por Código"><strong>Código</strong></label></td>
      <td valign="top" class="title"><?if ($ordemlista == '3') {$ordemlistadirnova = $ordemlistadir2;?><img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle"><?} else {$ordemlistadirnova = 'ASC';}?> <label onclick="ordena('3','<?=$ordemlistadirnova?>');" title="Ordenar por Título <?=$subtit2?>"><strong>Título <?=$subtit2?></strong></label></td>
	  <td valign="top" class="title"><strong>Responsabilidade</strong></td>
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
		 <td><a href="<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/programa/cadprograma&acao=C&prgid=<?=$prgid?>"><?=$codigo?></a></td>
		 <?if ($descricao=='') $descricao = '<font color="red">Não atribuído pelo SIGPLAN</font>'?>
		 <td onclick="abreconteudo('geral/listaacao.php?prgid=<?=$prgid?>&ordem=<?=$ordemgrupo?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>','<?=$i?>-<?=$codigo?>')"> <img src="../imagens/mais.gif" name="+" border="0" id="img<?=$i?>-<?=$codigo?>"> <?=$descricao?></td>
	     <td align=right><?$db->monta_lista_simples("select pfldsc from usuarioresponsabilidade u inner join perfil p on u.pflcod=p.pflcod where usucpf='".$_SESSION['usucpf']."' and rpustatus='A' and u.prgid='".$prgid."'","",100,20)?></td>
		 <td align=right>(<?=$numacoes?>)</td>
	 </tr>
	 <tr bgcolor="<?=$marcado?>">
		 <td></td>
		 <td colspan="2" id="td<?=$i?>-<?=$codigo?>"></td><td></td>
	 </tr>
<?}
if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';?>
    <tr class="<?=$marcado?>">
      <td valign="top" class="title"><strong></strong></td>
      <td colspan="2" valign="top" class="title"><strong>Total geral de Ações nos Programas</strong></td>
      <td valign="top" class="title" align='right'><strong><?=$totalacoes?></strong></td>
    </tr>
</tbody>
</form>
</table>

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
