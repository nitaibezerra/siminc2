<?php
header('content-type: text/html; charset=iso-8859-1;');
include "config.inc";
include APPRAIZ . 'includes/classes_simec.inc';
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include APPRAIZ . 'fabrica/classes/autoload.inc';
 
$idAuditoria = $_POST['idAuditoria'];

$historicoAuditoriaRepositorio = new HistoricoAuditoriaRepositorio();
$listaHistoricoAuditoria = $historicoAuditoriaRepositorio->recuperePorIdAuditoria($idAuditoria);
?>
<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
	<thead>
		<tr>
			<th>Realizado por</th>
			<th>Data modificação</th>
			<th>Motivo</th>
			<th>Observação</th>
			<th>Situação</th>
		</tr>
	</thead>
	<tbody>
	<?php 
		if (!empty($listaHistoricoAuditoria)) {
			foreach ($listaHistoricoAuditoria as $historicoAuditoria) { ?>
				<tr>
					<td><?php echo $historicoAuditoria->getFiscal()->getNome();?></td>
					<td><?php echo $historicoAuditoria->getDataAuditoriaFormatada("d/m/Y");?></td>
					<td><?php echo $historicoAuditoria->getMotivo();?></td>
					<td><?php echo $historicoAuditoria->getObservacao();?></td>
					<td><?php echo $historicoAuditoria->getSituacaoAuditoria() == SituacaoAuditoria::PENDENTE ? "Pendente" : "Concluída" ;?></td>
				</tr>
	<?php 	}
		} else {
	?>
		<tr>
			<td class="alignCenter" colspan="8">N&atilde;o foram encontrados registros</td>
		</tr>
	<?php }	?>
	
	</tbody>
</table>
