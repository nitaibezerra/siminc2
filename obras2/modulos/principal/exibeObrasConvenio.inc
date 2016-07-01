
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		  
<script>
  	function abrirobra(obrid){		  		
  		window.opener.location.href = '?modulo=principal/cadObra&acao=A&obrid='+obrid;
  		window.close();
  	}
</script>
<?php	
	$obra  = new Obras();
	$param = array('numconvenio' => $_REQUEST['covnumero']);
	$sql   = $obra->listaObraConvenio( $param );
//	$sql = "select 
//				'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" onclick=\"abrirobra(' || oi.obrid || ')\"/>' as acao,
//				oi.obrid,
//				'<a href=\"javascript:void(0)\" onclick=\"abrirobra(' || oi.obrid || ')\" style=\"color:black\">' || oi.obrdesc || '</a>',
//				ee.entnome,
//				so.stodesc,
//				oi.obrpercexec
//			from 
//				obras.obrainfraestrutura oi
//			inner join
//				entidade.entidade ee ON ee.entid = oi.entidunidade 
//			left join
//				obras.situacaoobra so ON so.stoid = oi.stoid
//			where 
//				numconvenio = '{$_REQUEST['covnumero']}'";
	
	//$rsObras = $db->carregar($sql);	
	
	monta_titulo('Lista de obras atendidas pelo convênio','Listagem');
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="subtituloDireita" width="120">Nº Convênio</td>
				<td>'.$_REQUEST['covnumero'].'</td>
			</tr>
		  </table>';
	
	$cabecalho = array('Ação','ID','Nome da Obra','Unidade Implantadora','Situação da Obra','% Executado');
	$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '', '', '');
	
	echo '<br/><center><input type="button" value="Fechar" onclick="window.close();" /></center>';
 ?>