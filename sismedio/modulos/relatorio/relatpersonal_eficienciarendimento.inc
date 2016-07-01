<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Eficiência/Rendimento</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
	<td class="SubTituloDireita">Agrupamento : </td>
	<td>
	<? 
	$t[] = array("codigo" => "1", "descricao" => "IES");
	$t[] = array("codigo" => "2", "descricao" => "UF");
	$db->monta_combo('agrupamento', $t, 'S', '', '', '', '', '', 'N', 'agrupamento', '', $_REQUEST['agrupamento']);
	?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita">Perfil : </td>
	<td>
	<? 
	$s[] = array("codigo" => PFL_PROFESSORALFABETIZADOR, "descricao" => "Professores");
	$s[] = array("codigo" => PFL_COORDENADORPEDAGOGICO, "descricao" => "Coordendores Pedagógicos");
	$s[] = array("codigo" => PFL_ORIENTADORESTUDO, "descricao" => "Orientadores de Estudo");
	$db->monta_combo('perfil', $s, 'S', '', '', '', '', '', 'N', 'perfil', '', $_REQUEST['perfil']);
	?>
	</td>
</tr>
</table>
<?

echo '<p align="center"><input type="button" value="Aplicar" onclick="window.location=\'sismedio.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_eficienciarendimento.inc&agrupamento=\'+document.getElementById(\'agrupamento\').value+\'&perfil=\'+document.getElementById(\'perfil\').value;this.disabled=true;"> <input type=button value="Limpar" onclick="window.location=\'sismedio.php?modulo=relatorio/relatoriospersonalizados&acao=A&buscar=1&relatorio=relatpersonal_eficienciarendimento.inc\';"></p>';


if($_REQUEST['agrupamento'] && $_REQUEST['perfil']) :

switch($_REQUEST['agrupamento']) :
	case '1':
		$agrup = "foo.unisigla";
		$cabecalho = array("IES","Nº de ingressantes","Nº de concluintes","%");
		break;
	case '2':
		$agrup = "foo.estuf";
		$cabecalho = array("UF","Nº de ingressantes","Nº de concluintes","%");
		break;
endswitch;

?>
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
<tr>
	<td>
	<?
	
	$sql = "select 
			{$agrup},
			sum(foo.total) as total,
			sum(foo.frequencia) as frequencia,
			round((sum(foo.frequencia)::numeric/sum(foo.total)::numeric)*100,2) as porcentfrequencia
			from (
			
			select uu.unisigla, 
			       m.estuf, 
			       1 as total, 
			       case when coalesce(c.cerfrequencia,0) > 75 then 1 else 0 end as frequencia 
			from sismedio.identificacaousuario i 
			inner join sismedio.tipoperfil t on t.iusd = i.iusd 
			inner join sismedio.universidadecadastro u on u.uncid = i.uncid 
			inner join sismedio.universidade uu on uu.uniid = u.uniid 
			inner join territorios.municipio m on m.muncod = i.muncodatuacao 
			left join sismedio.certificacao c on c.iusd = i.iusd and c.pflcod = t.pflcod 
			where i.iusstatus='A' and t.pflcod=".$_REQUEST['perfil']."
			
			) foo
			group by {$agrup} 
			order by {$agrup}";
	
	
	$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N');
	?>
	</td>

</tr>

</table>
<?

else :
 
echo '<p>Selecione o Agrupador e Perfil</p>';

endif;

?>