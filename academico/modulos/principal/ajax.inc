<?php
header( 'Content-type: text/html; charset=iso-8859-1' );

$sql = "
	SELECT
	COALESCE (sum(lp.lepvlrpublicacao), 0) as publicado,
	COALESCE (sum(lp.lepvlrhomologado), 0) as homologado,
	COALESCE (sum(lp.lepvlrprovefetivados), 0) as efetivado,
	
	cls.clsdsc
	FROM
		academico.lancamentoeditalportaria AS lp
		left JOIN 
			academico.cargos AS ca ON (ca.crgid  = lp.crgid)
		left JOIN 
			academico.classes AS cls ON (cls.clsid = ca.clsid )
		INNER JOIN 
			academico.editalportaria AS eppub ON (eppub.edpid = lp.edpid AND lp.edpid = 692) 
		
	WHERE
		lp.lepano = '2008'
	GROUP BY cls.clsdsc
";

$dados = $db->pegaLinha($sql);


?>

<br/>
<table border="0">
	<tr>
		<td><b>Docentes:</b></td>
		<td><?= $$dados['docentes'] ?></td>		
	</tr>
	<tr>
		<td><b>TA Nível E:</b></td>
		<td><?= $$dados['tece'] ?></td>		
	</tr>
	<tr>
		<td><b>TA Nível D:</b></td>
		<td><?= $$dados['tecd'] ?></td>		
	</tr>
	<tr>
		<td><b>TA Nível C:</b></td>
		<td><?= $$dados['tecc'] ?></td>		
	</tr>
	<tr>
		<td><b>TA Nível B:</b></td>
		<td><?= $$dados['tecb'] ?></td>		
	</tr>
</table>