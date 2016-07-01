<?php

/*
Sistema Simec
Setor responsavel: SPO-MEC
Desenvolvedor: Equipe Consultores Simec
Analista: Gilberto Arruda Cerqueira Xavier
Programador: Renan de Lima (renandelima@gmail.com)
Módulo:verequipe.inc
Finalidade: este arquivo permite ver os participantes de uma reunião
*/

	// captura os participantes de uma agenda de reunião
	$agrid = (integer) $_REQUEST['agrid'];	
	$sql =
		"select " .
			" distinct u.usucpf, " .
			" u.usuemail, " .
			" u.usunome || ' (' || o.orgdsc || ')' as participante, " .
			" u.usufoneddd || '-' || u.usufonenum as fone " .
		" from agenda_reuniao_participante arp " .
			" inner join seguranca.usuario u on u.usucpf = arp.usucpf " .
			" inner join orgao o on o.orgcod = u.orgcod " .
		" where " .
			" arp.agrid = " . $agrid .
		" order by participante";
	 $rs = $db->carregar( $sql );
	 
?>
<?php if (  $rs && count( $rs ) > 0 ) : ?>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<table  class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center" style="color:#808080;">
		<?php foreach ( $rs as $linha ) : ?>
			<tr>
				<td>
					<?= $linha['participante'] ?>
					Telefone: <?= $linha['fone'] ?>
				</td>
			</tr>
		<?php endforeach; ?>
	</table>
<?php else : ?>
	<p style="color:red" align="center"><br/><br/><br/>Nenhum participante</p>
<?php endif; ?>