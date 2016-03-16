<html>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<body>
<?php

	
	$sql = "SELECT 
				'<center><img src=\"../imagens/alterar.gif\" onclick=\"abrePTAconsolidado(\''||p.ptrid||'\', \'pta\');\" style=\"cursor:pointer;\" border=\"0\"></center>' as acao,
				-- p.ptrid,
			    case when p.ptridpai is not null then tf.trefnome else 'Original' end as tipo,
			    case when p.ptrstatus = 'A' then 'Ativo' else 'Inativo' end as status,
			    case 
			    	when p.ptrsituacao = 'A' then 'Aprovado'
			    	when p.ptrsituacao = 'R' then 'Reprovado'
			    	when p.ptrsituacao = 'P' then 'Pendente'
			    	when p.ptrsituacao = 'C' then 'Em Cadastramento'
			    	when p.ptrsituacao = 'E' then 'Em Análise'
			    	else ''
			    end as situacao,
			    to_char(ptrdataalteracao, 'DD/MM/YYYY') as data
			FROM 
			  	emenda.planotrabalho p
			LEFT JOIN emenda.ptminreformulacao mr ON p.ptrid = mr.ptrid
			LEFT JOIN emenda.reformulatipos rt ON mr.refid = rt.refid
			LEFT JOIN emenda.tiporeformulacao tf ON rt.trefid = tf.trefid
			WHERE
				/*p.ptrstatus = 'A'
				and */p.ptrcod = (  SELECT ptrcod FROM emenda.planotrabalho WHERE ptrid = ".$_SESSION['emenda']['ptridAnalise']." /*and ptrstatus = 'A'*/ )
			ORDER BY
				p.ptrid";

	//$arDados = $db->carregar( $sql );
	
	/*foreach ($arDados as $key => $v) {
		$registro[$key] = array(
								 "acao" => '<center><img src="../imagens/alterar.gif" onclick="abrePTAconsolidado(\''.$v['ptrid'].'\', \'pta\');" style="cursor:pointer;" border="0"></center>',
								 "ptrid" => $v['ptrcod']."/".$_SESSION['exercicio'],
								 "tabnome" => $v['tabnome'],
								 "histtxtalteracao" => $v['histtxtalteracao'],
								 "histdatainclusao" => $v['histdatainclusao'],
								 "usunome" => $v['usunome']
								);
	}*/
	monta_titulo('Histórico Plano de Trabalho','');
//	$cabecalho = array("Ação","PTA", "Tipo de Reformulação", "Status", "Situação");
	$cabecalho = array("Ação","Tipo de Reformulação", "Status", "Situação", "Data");
	$alinhamento = array( 'center', 'center', 'center', 'center', 'center', 'center' );
	$db->monta_lista( $sql, $cabecalho, 20, 4, 'N','Center','','form', $tamanho, $alinhamento);
	
?>
</body>
</html>
