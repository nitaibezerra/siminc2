<?
function exibirPeriodoReferencia($dados) {
	global $db;

	if($dados['percod']) {
		
		$sql = "
          SELECT
               id_periodo_referencia AS percod,
               titulo AS pertitulo,
               descricao AS perdescricao,
               to_char(inicio_validade, 'DD/MM/YYYY') AS periniciovalidade,
               to_char(fim_validade, 'DD/MM/YYYY') AS perfimvalidade,
               to_char(inicio_preenchimento, 'DD/MM/YYYY') AS perinicioaberturapreenchimento,
               to_char(fim_preenchimento, 'DD/MM/YYYY') AS perfimaberturapreenchimento,
               exibir_indicadores AS perexibirindicadores
          FROM planacomorc.periodo_referencia
          WHERE
            id_periodo_referencia = {$dados['percod']}
        ";

		$periodoreferencia = $db->pegaLinha($sql);
        if ($periodoreferencia) {
            foreach ($periodoreferencia as $k => $v) {
                $periodoreferencia[$k] = $v;//utf8_encode($v);
            }
            echo simec_json_encode($periodoreferencia);
        }
    }
}

function atualizarPeriodo($dados) {
	global $db;
	
	$sql = "UPDATE planacomorc.periodo_referencia
                  SET titulo = '".$dados['pertitulo']."', 
                      descricao = '".$dados['perdescricao']."', 
                      inicio_validade = '".formata_data_sql($dados['periniciovalidade'])."',
                      fim_validade = '".formata_data_sql($dados['perfimvalidade'])."', 
                      inicio_preenchimento = '".formata_data_sql($dados['perinicioaberturapreenchimento'])."',
                      fim_preenchimento = '".formata_data_sql($dados['perfimaberturapreenchimento'])."',
                      exibir_indicadores  =".(isset($dados['perexibirindicadores']) ? 'TRUE' : 'FALSE')."
                  WHERE id_periodo_referencia = '".$dados['percod']."'";
	
	$db->executar($sql);

	$db->commit();

    $return = array(
        "alerta" => utf8_encode("Período de Referência alterado com sucesso"),
        "reload" => "location.reload();"
    );

    echo simec_json_encode($return);
}

function inserirPeriodo($dados) {
	global $db;

    $perexibirindicadores = isset($dados['perexibirindicadores']) ? 't' : 'f';
	
	$sql = "INSERT INTO planacomorc.periodo_referencia(descricao, titulo, inicio_validade, fim_validade, 
                                                           inicio_preenchimento, fim_preenchimento, id_exercicio,
                                                           instante_criacao, exibir_indicadores)
    VALUES ('".$dados['perdescricao']."', '".$dados['pertitulo']."', 
    		'".formata_data_sql($dados['periniciovalidade'])."', 
    		'".formata_data_sql($dados['perfimvalidade'])."', 
            '".formata_data_sql($dados['perinicioaberturapreenchimento'])."', 
            '".formata_data_sql($dados['perfimaberturapreenchimento'])."', 
            '".$_SESSION['exercicio']."', 
            NOW(), '{$perexibirindicadores}')";

    //ver($sql, d);
	
	$db->executar($sql);

	$db->commit();

	$return = array(
        "alerta" => utf8_encode("Período de Referência inserido com sucesso"),
		"reload" => "location.reload();"
    );

    echo simec_json_encode($return);
}

/*
function excluirPeriodo($dados) {
	global $db;
	$sql = "UPDATE planacomorc.produtosubacao SET psbstatus='I' WHERE psbid='".$dados['psbid']."'";
	$db->executar($sql);
	$db->commit();

    $return = array(
        "alerta" => utf8_encode("Período de Referência excluído com sucesso"),
        "reload" => "location.reload();"
    );

    echo simec_json_encode($return);
}*/

function excluirPeriodoReferencia($dados) {
    global $db;
    $sql = "delete from planacomorc.periodo_referencia where id_periodo_referencia='".$dados['percod']."'";
    $db->executar($sql);
    $db->commit();

    $alert = array(
        "alert" => "Período de Referência excluído com sucesso",
        "location" => "planacomorc.php?modulo=principal/periodoreferencia/listaperiodoreferencia&acao=A"
    );

    alertlocation($alert);
}

?>