<?
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
	
	//$sql = "select p.pjeid, p.pjecod, p.pjedsc as descricao from monitora.projetoespecial p where prsano=".$_SESSION[ 'exercicio']." and pjestatus='A' order by p.pjedsc";	
	//$sql = "(select p.pjeid, p.pjecod, p.pjedsc as descricao from monitora.projetoespecial p inner join monitora.usuario_projeto u on p.pjeid = u.pjeid where p.prsano=".$_SESSION['exercicio']." and p.pjestatus='A' and u.usucpf = '".$_SESSION['usucpf']."' order by p.pjedsc)
	//		union
	//		(select p.pjeid, p.pjecod, p.pjedsc as descricao from monitora.projetoespecial p inner join monitora.usuarioresponsabilidade u on p.pjeid = u.pjeid where p.prsano=".$_SESSION['exercicio']." and p.pjestatus='A' and u.usucpf = '".$_SESSION['usucpf']."' order by p.pjedsc)";
	
	$sql = "select pe.pjeid, trim(ug.ungabrev) ||'-'|| trim(pe.pjecod) as codigopje, trim(pe.pjedsc) as descricao from ((monitora.projetoespecial pe inner join unidadegestora ug using (ungcod)) left join monitora.usuarioresponsabilidade ur using (pjeid)) where ur.usucpf = '".$_SESSION['usucpf']."' or pe.pjesnvisivel='t' group by pe.pjecod, pe.pjeid, ug.ungabrev, pe.pjedsc order by pjecod ";
	$rs = $db->carregar( $sql );
	//dbg($sql,1);
	
	$titulo_modulo='Lições Aprendidas em Projetos Especiais';
	monta_titulo($titulo_modulo,'');
	?>
	
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
	<thead>
    <tr>
      <td valign="top" class="title"><strong>Ações</strong></td>
      <td valign="top" class="title"><strong>Código</strong></td>
      <td valign="top" class="title"><strong>Descrição</strong></td>
      <td valign="top" class="title"><strong>Experiência</strong></td>
    </tr>
	</thead>
	<tbody>
	<? if ($rs)
	{
		$i = 0;
		foreach( $rs as $linha ){
			foreach($linha as $k=>$v) ${$k}=$v;
			if (fmod($i,2) == 0) $marcado = '' ; else $marcado='marcado';
				
	
				//calculo o numero de lições positivas e negativas
				$sql2 = "select pjeid, tpasnacerto, count(lapid) as total from monitora.licaoaprendida where pjeid = ".$pjeid." and lapstatus = 'A' group by pjeid, tpasnacerto";
				$rs2 = $db->carregar($sql2);
				$lpositiva = 0;
				$lnegativa = 0;
				$linha2 = null;
				$j=0;
				if ($rs2)
				{
					foreach( $rs2 as $linha2 ){
						if( $linha2["tpasnacerto"] == 't')
						{
							$lpositiva = (float)$linha2["total"];
						}else
						{
							$lnegativa = (float)$linha2["total"];
						}
					$j++;
					}
				}
				
				$ltotal = $lpositiva + $lnegativa;
				
				
				//calcula a porcentagem
				$pnegativa = (float)($lnegativa == 0?0:($lnegativa*100)/($lnegativa+$lpositiva));				
				$ppositiva =(float)($lpositiva == 0?0:($lpositiva*100)/($lnegativa+$lpositiva));				
				
				print '<tr class="'.$marcado.'"><td><img border="0" src="../imagens/alterar.gif" title="Ver lições aprendidas neste projeto." onclick="ver_proj('."'".$pjeid."'".')"></td><td>'.$codigopje.'</td><td>'.$descricao.'</td><td><font color="#009900">Positivas: '.sprintf("%05.2f",$ppositiva).'%</font><font color="#FF0000">   Negativas: '.sprintf("%05.2f",$pnegativa).'%<font></td></tr>';	  	  			
				
			$i++;}/*Fim do foreach $rs*/ 
			
	}else{?>
	<tr bgcolor="<?=$marcado?>">
	  <td class="title" style="color:red;">Nenhum projeto atribuido</td>
	  <td></td>
	</tr>	
	<?}?>
	</tbody>
</table>
<script>

	function ver_proj(cod) {
  		location.href ='monitora.php?modulo=principal/projespec/licaoaprendida&acao=A&pjeid='+cod;
  	}  
</script>
