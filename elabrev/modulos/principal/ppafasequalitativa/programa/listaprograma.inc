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
$agrupadopor = 'Ação';

//Ordem da listagem
if (!$ordemlista) $ordemlista = 2;

//Direcionamento da ordenação ASC / DESC
if ($ordemlistadir <> 'DESC') {$ordemlistadir = 'ASC';$ordemlistadir2 = 'DESC';} else $ordemlistadir2 = 'ASC';

//Controla se vai aparecer os programas baseado na ação ou no parâmetro
if (!$listaprg and $acao=='A') $listaprg='N'; elseif (!$listaprg) $listaprg='S';

//Se não tiver escolhido nehum tipo traz apenas normais
$tipoacao1='N';$tipoacao2='R';$tipoacao3='E';

//filtros de tipo de ação
$wh = "";

//Cabeçalho do sistema
include APPRAIZ."includes/cabecalho.inc";

?>
<br>
<?
if ($listaprg=='N')
{
	if ($ordemgrupo=='A')
	{
		$subtit1 = 'de Ações'; 
		$subtit2 = 'da Ação'; 
		$sql = "select a.prgcod, a.acacod as codigo, a.acadsc as descricao, a.prsano, count(*) as numacoes from elabrev.ppaacao_proposta a where a.acastatus='A'  and a.prsano = '".$_SESSION['exercicio']."' ".$wh." group by a.prgcod, a.acacod , a.acadsc, a.prsano order by ".$ordemlista." ".$ordemlistadir;
	}
}
else
{
	$tipoprgaca = "Programas";
	$sql = "select p.prgid, p.prgcod as codigo, prgdsc as descricao, p.prsano, count(*) as numacoes from elabrev.ppaprograma_proposta p inner join elabrev.ppaacao_proposta a on p.prgid=a.prgid where p.prgstatus='A'  and p.prsano = '".$_SESSION['exercicio']."'  ".$wh." group by p.prgid, p.prgcod , p.prgdsc, p.prsano order by ".$ordemlista." ".$ordemlistadir;
	
	$subtit1 = 'de Programas'; 
	$subtit2 = 'do Programa';
}

$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$db->cria_aba($abacod_tela,$url,'');
monta_titulo('Elaboração e Revisão de Programas e Ações','Clique no código entrar da elaboração e revisão - Total '.$subtit1.' ('.($nlinhas+1).')');?>

<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
		<tr>
			<td valign="top" width="10%">
				<?      
					if ($ordemlista == '2')
					{
	         	   		$ordemlistadirnova = $ordemlistadir2;
						?>
							<img src="../imagens/seta_ordem<?=$ordemlistadir?>.gif" width="11" height="13" align="middle">
						<?
					} 
					else
					{
						$ordemlistadirnova = 'ASC';
					}
				?> 
				<label onclick="ordena('2','<?=$ordemlistadirnova?>');" title="Ordenar por Código">
					<strong>
						Código
					</strong>
				</label>
			</td>
			<td valign="top"  width="80%">
				<strong>
					Título do Programa
				</strong>
			</td>
			<td valign="top"  width="10%">
				<strong>
					Total Ações
				</strong>
			</td>
		</tr>
	</thead>
	<tbody>
	<?
		$totalacoes = 0;
		for ($i=0; $i<=$nlinhas;$i++)
		{
  			$res = $db->carrega_registro($RS,$i);
			
  			// a linha abaixo transforma em variáveis todos os campos do array
  			if(is_array($res)) foreach($res as $k=>$v) ${$k}=$v;
  			
			$totalacoes = $totalacoes + $numacoes;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
			?>
			<tr 
				bgcolor="<?=$marcado?>" 
				onmouseover="this.bgColor='#ffffcc';" 
				onmouseout="this.bgColor='<?=$marcado?>';"
			>
				<td>
					<?
						if ($listaprg=='S')
						{
						?>
							<a 
								href="<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/ppafasequalitativa/programa/cadprograma&acao=C&prgid=<?=$prgid?>"
							>
								<?=$codigo?>
							</a>
						<?
						}
						else
						{
							if ($ordemgrupo=='A')
								print $prgcod.'.'.$codigo; 
							else
								print $codigo;
						}
					?>
				</td>
				<?
					if ($descricao=='')
					{
						$descricao = '<font color="red">Não atribuído pelo SIGPLAN</font>';
					}
				?>
			 	<td onclick="abreconteudo(<?
					if ($listaprg=='S')
					{
						?>'geral/listaacao3.php?prgid=<?=$prgid?>&ordem=<?=$ordemgrupo?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>'<?
					}
					else
					{
						?>'geral/listaacao3.php?codigo=<?if ($ordemgrupo=='A') print $prgcod.'.'.$codigo; else print $codigo;?>&ordem=<?=$ordemgrupo?>&tipoacao1=<?=$tipoacao1?>&tipoacao2=<?=$tipoacao2?>&tipoacao3=<?=$tipoacao3?>'<?
						}
					?>,'<?= $i ?>-<?= $codigo ?>')"<?
						if ($listaprg=='N') print 'style="color:#003c7b"';
					?>
				> 
					<img src="../imagens/mais.gif" name="+" border="0" id="img<?=$i?>-<?=$codigo?>"> 
					<?=$descricao?>
				</td>
				<td align='left'>
					(<?=$numacoes?>)
				</td>
			</tr>
			<tr bgcolor="<?=$marcado?>">
				<td>
				</td>
				<td id="td<?=$i?>-<?=$codigo?>">
				</td>
				<td>
				</td>
			</tr>
			<?
		}
		if ( fmod($i,2) == 0)
		{
			$marcado = '' ; 
		}
		else
		{
			$marcado='marcado';
		}
		?>
		<tr class="<?=$marcado?>">
			<td></td>
			<td valign="top" > 
				<table width="100%">
					<tr>
						<td>
							<strong>
								Total <?=$subtit1?>: <?=$i?>
							</strong>
						</td>
						<td align="right">
							<strong>
								Total de Ações:
							</strong>
						</td>
					</tr>
				</table>
			</td>
			<td align="left">
				<strong>
					<?=$totalacoes?>
				</strong>
			</td>
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