<?php
 
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

// Recupera todas as UFs das secretarias disponíveis
$sql = "SELECT 
			ende.estuf
		FROM entidade.entidade ent 
		LEFT JOIN entidade.funcaoentidade 	fen ON fen.entid = ent.entid 
		LEFT JOIN entidade.endereco 		ende ON ende.entid = ent.entid
		WHERE fen.funid = 6 
		AND entstatus = 'A' 
		AND ende.tpeid = 2
		AND entnumcpfcnpj IS NOT NULL
		ORDER BY estuf";

$arSecretariasDisponiveis = $db->carregar($sql);

// Cria array de ufs das secretárias atuais
if(count($arSecretariasDisponiveis)){
	foreach($arSecretariasDisponiveis as $dados){
		$arUFs[] = strtoupper($dados['estuf']);
	}
}

// Recupera a planilha dos novos secretários
$sql = "SELECT 
			*			 
		FROM carga.carga_secretarios_2011";

$arNovosSecretarios = $db->carregar($sql);

// Percorre a planilha dos novos secretários
if(count($arNovosSecretarios)){
	foreach($arNovosSecretarios as $secretario){
		
		// Insere a secretaria se não existir
		if(!in_array($secretario['uf'], $arUFs)){
			
			$sql = "INSERT INTO entidade.entidade 
						(entnumcpfcnpj, entnome, entstatus)
					VALUES
						('{$secretario['cnpj']}','SECRETARIA ESTADUAL DE EDUCAÇÃO - {$secretario['uf']}','A') RETURNING entid";
			
			$id = $db->pegaUm($sql);	
			
			$sql = "INSERT INTO entidade.endereco 
						(entid, tpeid, endcep, endlog, muncod, estuf, endstatus)
					VALUES
						('{$id}','2','{$secretario['cep']}', '".str_replace("'","\'",$secretario['endereco'])."', '{$secretario['muncod']}', '{$secretario['uf']}', 'A')";
			
			$db->executar($sql);
			
			$sql = "INSERT INTO entidade.funcaoentidade 
						(funid, entid, fuedata, fuestatus)
					VALUES
						('6','{$id}','".date('Y-m-d H:i:s')."','A')";
			
			$db->executar($sql);
			
			$arMsg[]['insert'] =  "{$secretario['uf']} - Foi criada a secretaria entid {$id}";
		}
		
		// Recupera secretário 
		$sql = "SELECT
					ent.entid,
					ent.entnome, 
					ent.entnumcpfcnpj,
					fue.fueid
				FROM entidade.entidade ent
				LEFT JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND funid = 25 
				WHERE ent.entnumcpfcnpj = '{$secretario['cpf']}'";
		
		$arSecretarioAtual = $db->pegaLinha($sql);
		
		// Insere os cps que não existem
		if(!$arSecretarioAtual['entnumcpfcnpj']){
			
			$sql = "INSERT INTO entidade.entidade
						(entnome, entnumcpfcnpj, entemail, entnumdddcomercial, entnumcomercial, entstatus)
					VALUES
						('".str_replace("'","\'",$secretario['nome'])."', '{$secretario['cpf']}', '{$secretario['email1']}', '{$secretario['ddd']}', '{$secretario['fone1']}', 'A')
					RETURNING entid";
			
			$entid = $db->pegaUm($sql);
			
			$sql = "INSERT INTO entidade.endereco
						(entid, tpeid, endcep, endlog, muncod, estuf, endstatus)
					VALUES
						('{$entid}', '2', '{$secretario['cep']}', '".str_replace("'","\'",$secretario['endereco'])."', '{$secretario['muncod']}', '{$secretario['uf']}', 'A')";
			
			$db->executar($sql);
			
			$sql = "INSERT INTO entidade.funcaoentidade
						(funid, entid, fuedata, fuestatus)
					VALUES
						('25', '{$entid}', now(), 'A') RETURNING fueid";
			
			$fueid = $db->pegaUm($sql);			
			
			$sql = "SELECT 
						ent.entid 
					FROM entidade.entidade ent
					INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND funid = 6
					INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
					WHERE ende.estuf = '{$secretario['uf']}'";
			
			$entid_secretaria = $db->pegaUm($sql);
			
			$sql = "INSERT INTO entidade.funentassoc 
						(entid, fueid, feadata)
					VALUES
						('{$entid_secretaria}', '{$fueid}', now())";
			$db->executar($sql);
			
			$arMsg[]['insert'] = "{$secretario['uf']} - Inseriu o secretário {$secretario['nome']} - cpf {$secretario['cpf']} entid {$entid}";
			
		// Insere a função entidade 25 se não existir para a entidade
		}elseif($arSecretarioAtual['entnumcpfcnpj'] && !$arSecretarioAtual['fueid']){
			
			$sql = "INSERT INTO entidade.funcaoentidade
						(funid, entid, fuedata, fuestatus)
					VALUES
						('25', '{$arSecretarioAtual['entid']}', now(), 'A')";
			
			$fueid = $db->pegaUm($sql);
			
			$arMsg[]['insert'] = "{$secretario['uf']} - Inseriu a função entidade fueid {$fueid} para {$arSecretarioAtual['entnome']}";
		}
	}
}

// Recupera secretários e secretarias atuais
$sql = "SELECT DISTINCT
			ent.entid as codigo,
			ent2.entid,
			eed.estuf, 
			ent.entnome as secretario,
			ent.entnumcpfcnpj as cpf, 
			ent2.entnome as secretaria,
			ent2.entnumcpfcnpj as cnpj,
			efu.fueid, 
			efu.fuestatus,
			fea.feaid,
			ent.entstatus
		FROM entidade.entidade ent
			INNER JOIN entidade.endereco 		eed ON eed.entid = ent.entid
			INNER JOIN entidade.funcaoentidade 	efu ON efu.entid = ent.entid AND efu.funid = 25 --AND efu.fuestatus = 'A'
			INNER JOIN entidade.funcao 			fun ON fun.funid = efu.funid
			LEFT JOIN entidade.funentassoc 		fea ON fea.fueid = efu.fueid
			LEFT JOIN entidade.entidade         ent2 ON ent2.entid = fea.entid 
			LEFT JOIN entidade.endereco         ende2 ON ende2.entid = ent2.entid 
			LEFT JOIN entidade.funcaoentidade 	efu2 ON efu2.entid = ent2.entid AND efu2.funid = 6 --AND efu2.fuestatus = 'A'
			LEFT JOIN entidade.funcao 			fun2 ON fun2.funid = efu2.funid
		--WHERE ent.entstatus = 'A'
		ORDER BY eed.estuf";

$arSecretariasAtuais = $db->carregar($sql);

// Percorre as secretarias atuais
if($arSecretariasAtuais){
	foreach($arSecretariasAtuais as $dados){
		
		$sql = "SELECT 
					feaid 
				FROM entidade.funentassoc 
				WHERE fueid = {$dados['fueid']}";
		
		$feaid = $db->pegaUm($sql);
		
		if(!$feaid){
			
			$sql = "SELECT 
						ent.entid 
					FROM entidade.entidade ent
					INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid AND funid = 6
					INNER JOIN entidade.endereco ende ON ende.entid = ent.entid 
					WHERE ende.estuf = '{$dados['estuf']}'";
			
			$entid_secretaria = $db->pegaUm($sql);
			
			$sql = "INSERT INTO entidade.funentassoc 
						(entid, fueid, feadata)
					VALUES
						('{$entid_secretaria}', '{$dados['fueid']}', now())";
			$db->executar($sql);
		}
		
		$sql = "SELECT 
					* 
				FROM carga.carga_secretarios_2011 
				WHERE cpf = '".strtoupper($dados['cpf'])."'";
		
		$arRecuperaNovoSecretario = $db->pegaLinha($sql);
		
		if($arRecuperaNovoSecretario['cpf'] == $dados['cpf'] && $dados['entstatus'] != 'A'){
			$sql = "UPDATE entidade.entidade SET entstatus = 'A' WHERE entid = {$dados['codigo']}";
			$db->executar($sql);
			$arMsg[]['update'] = "{$dados['estuf']} - Ativou a entidade {$dados['secretario']} - cpf {$dados['cpf']} fueid {$dados['fueid']} entid {$dados['codigo']}";
		}
		
		// Caso o cpf seja diferente desativa a entidade atual
		if($arRecuperaNovoSecretario['cpf'] == $dados['cpf'] && $dados['fuestatus'] != 'A'){
			
			$sql = "UPDATE entidade.funcaoentidade SET fuestatus = 'A' WHERE fueid = {$dados['fueid']}";
			$db->executar($sql);
			
			$arMsg[]['update'] = "{$dados['estuf']} - Ativou o secretario {$dados['secretario']} - cpf {$dados['cpf']} fueid {$dados['fueid']} entid {$dados['codigo']}";
			
		}else{
			
			$sql = "UPDATE entidade.funcaoentidade SET fuestatus = 'I' WHERE fueid = {$dados['fueid']}";
			$db->executar($sql);
			
			$arMsg[]['update'] = "{$dados['estuf']} - Desativou o secretario {$dados['secretario']} - cpf {$dados['cpf']} fueid {$dados['fueid']} entid {$dados['codigo']}";
			
		}
	}
}

// Exibe as execuções
if(count($arMsg)){
	$x=0;
	$y=0;
	foreach($arMsg as $dados){
		
//		echo $x." - ".$dados['insert']."<br/>";

		if($dados['insert']){
			$html1 .=  $x." - ".$dados['insert']."<br/>";		
			$x++;
		}
		
		if($dados['update']){
			$html2 .=  $y." - ".$dados['update']."<br/>";		
			$y++;
		}		
	}
}

echo "<h3>Insert</h3>".$html1."<h3>Update</h3>".$html2;

$db->commit();

?>