<?php

header( 'Content-type: text/html; charset=iso-8859-1' );

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include_once APPRAIZ . "sca/classes/CrachaProvisorio.class.inc";
include_once APPRAIZ . "sca/classes/Visita.class.inc";
include_once APPRAIZ . "sca/classes/Visitante.class.inc";
include_once APPRAIZ . "sca/classes/Equipamento.class.inc";

$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

$oVisitante = new Visitante();
$oVisita = new Visita();
$oEquipamento = new Equipamento();

if($_REQUEST['servico'] == 'recuperarFoto'){

    @session_start();
    unset($_SESSION['imagemVisitante']);

    $vttid = $_REQUEST['vttid'];
    $nu_matricula_siape = $_REQUEST['nu_matricula_siape'];

    $idArquivo = $oVisitante->recuperarIdFoto($vttid, $nu_matricula_siape);

    if($idArquivo){
        $foto = new FilesSimec("visitantefoto", array() ,"sca");

        if($foto->existeArquivo($idArquivo)){
?>
    <img src="?modulo=principal/imagem&acao=A&idArquivo=<?=$idArquivo?>"
         alt="Foto Visitante" width="320px" height="240px"
         id="imagemVisitante" />
<?php
            return;
        }
    }
?>
    <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"
        id="CapturaFoto" width="320" height="240"
        codebase="http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab">
        <param name="movie" value="componentes/CapturaFoto.swf" />
        <param name="quality" value="high" />
        <param name="bgcolor" value="#ffffff" />
        <param name="allowScriptAccess" value="sameDomain" />
        <embed src="componentes/CapturaFoto.swf" quality="high" bgcolor="#ffffff"
            width="320" height="240" name="CapturaFoto" align="middle"
            play="true" loop="false" quality="high" allowScriptAccess="sameDomain"
            type="application/x-shockwave-flash" pluginspage="http://www.adobe.com/go/getflashplayer">
        </embed>
    </object>
<?php
}
else if($_REQUEST['servico'] == 'recuperarEdificio'){

?>
    <table border="0" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <th colspan="2">Edifício</th>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="168px">Edifício:</td>
            <td><?=$oVisita->montaComboEdificio();?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="168px" >Andar:</td>
            <td><?=$oVisita->montaComboAndarEdificio($edfid, $edaid);?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="168px" >Destino:</td>
            <td><?=$oVisita->montaComboDestino();?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="168px" >Sala:</td>
            <td><?=campo_texto('numsalforahorario','S','S','Informe o número da sala',10,10,'','','left','',0,"id='numsalforahorario'");?></td>
        </tr>
    </table>
<?php
}
else if($_REQUEST['servico'] == 'verificaSeVisitanteFoiRegistradoPeloNumeroEtiquetaManual'){
	echo simec_json_encode(array("registrado" => $oVisita->verificaSeVisitanteFoiRegistradoPeloNumeroEtiquetaManual($_REQUEST['vstnumcracha'])));
}
else if($_REQUEST['requisicao'] == 'reimprimir'){
	$ativo = $oVisita->verificaSeVisitaEstaAtivaPeloIdVisitante($_REQUEST['vttid']);
	$visita = array("ativo"=>$ativo);
	
	if ($ativo){
		$visitaAtiva    = $oVisita->recuperaUltimaVisitaAtivaPeloIdVisitante($_REQUEST['vttid']);
                $VisitanteNome  = $oVisita->recuperarVisitante($_REQUEST['vttid'], null);
		
                $visitaAtiva['vttnome'] = $VisitanteNome['vttnome'];   
		$visitaAtiva['edfid']   = $_REQUEST['edfid'];
		$visitaAtiva['edaid']   = $_REQUEST['edaid'];
		$visitaAtiva['dstid']   = $_REQUEST['dstid'];
		
                //echo "<pre>";  print_r($visitaAtiva);exit;
		$etiquetaVisitante =  $oVisitante->geraEtiqueta( $visitaAtiva );
	}
	
	$etiqueta = array("valor"=>$etiquetaVisitante);
	echo simec_json_encode(array("visita"=>$visita, "etiqueta"=>$etiqueta));
}else if($_REQUEST['servico'] == 'verificarPessoaServidor'){

    $vttdoc = $_REQUEST['vttdoc'];
    $resultado = $oVisita->verificarPessoaServidor($vttdoc);

    echo simec_json_encode($resultado);
}else if($_REQUEST['servico'] == 'consultarVisitanteDuplicadoEntrada'){

    $vttdoc = $_REQUEST['vttdoc'];
    $tipo = $_REQUEST['tipo'];
    $resultado = $oVisita->consultarVisitanteDuplicadoEntrada($vttdoc, $tipo);

    echo simec_json_encode($resultado);
}
else if($_REQUEST['servico'] == 'consultarVisitanteEntrada'){

    $vttid = $_REQUEST['vttid'];
    $vttdoc = $_REQUEST['vttdoc'];
    $nu_matricula_siape = $_REQUEST['nu_matricula_siape'];
    $tipo = $_REQUEST['tipo'];
    $resultado = $oVisita->consultarVisitanteEntrada($vttid, $vttdoc, $nu_matricula_siape, $tipo);

    echo simec_json_encode($resultado);
}
else if($_REQUEST['servico'] == 'consultarPessoaForaHorario'){

	$documento = $_REQUEST['documento'];
	$resultado = $oVisita->consultarPessoaForaHorario($documento);

	echo simec_json_encode($resultado);
}
else if($_REQUEST['servico'] == 'buscarAndarEdificio'){

    $edfid = $_REQUEST['edfid'];

    $andares = $oVisita->recuperarAndaresEdificio($edfid);

    $resultado = array(
        'andares' => $andares,
        'status'  => 'ok');

    echo simec_json_encode($resultado);
}
//caso a chamada do ajax seja da busca de equipamentos da tela de registrar entrada de visitante
else if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'buscarEquipamento'){

    $eqmnumetiqueta = $_REQUEST['etiqueta'];

    $equipamento = $oEquipamento->consultarEquipamento($eqmnumetiqueta);

    $equipamentoPessoa = $oVisita->recuperarPessoaPorEquipamento($eqmnumetiqueta);
    
    if($equipamentoPessoa) {
    	$pessoa = $oVisita->consultarVisitanteEntrada($equipamentoPessoa['vttid'], null, $equipamentoPessoa['nu_matricula_siape'], 'T');
    }
    
    if($equipamento){
        $resultado = array(
            'codigo'    => $equipamento['codigo'],
            'descricao' => utf8_encode($equipamento['descricao']),

        	'vttid'              => $pessoa['vttid'],
        	'nu_matricula_siape' => $pessoa['nu_matricula_siape'],
        	'vttdoc'             => $pessoa['vttdoc'],
        	'vttnome'            => utf8_encode($pessoa['vttnome']),
        	'vttobs'             => utf8_encode($pessoa['vttobs']),
        	'status'    => 'ok');
    } else{
        $resultado = array('status' => 'erro');
    }
    echo simec_json_encode($resultado);
}
else if($_REQUEST['servico'] == 'validarEntradaVisitante'){

    $vttid = $_REQUEST['vttid'];
    $nu_matricula_siape = $_REQUEST['nu_matricula_siape'];
    $vstnumcracha = $_REQUEST['vstnumcracha'];
    $edaid = $_REQUEST['edaid'];
    $equipamentos = $_REQUEST['equipamentos'];

    $validacao = $oVisita->validarEntrada($vttid, $nu_matricula_siape, $vstnumcracha, $edaid, explode(",", $equipamentos));

    if(empty($validacao)){
        $resultado = array('status'  => 'ok');
    } else{
        $resultado = array('status' => 'erro'
                          ,'mensagem' => utf8_encode($validacao));
    }

    echo simec_json_encode($resultado);

}else if($_REQUEST['servico'] == 'consultarVisitanteSaida'){

    $vttid = $_REQUEST['vttid'];
    $vttdoc = $_REQUEST['vttdoc'];
    $nu_matricula_siape = $_REQUEST['nu_matricula_siape'];
    $tipo = $_REQUEST['tipo'];
    $vstid = $_REQUEST['vstid'];

    if($tipo == 'T'){
        $pessoa = $oVisita->recuperarPessoa($vttid, $vttdoc, $nu_matricula_siape);

        if($pessoa){
            $array_equipamentos = array();
            $equipamentos = $oVisita->consultarSaidaEquipamento($pessoa['vttid'], $pessoa['nu_matricula_siape']);
            if($equipamentos && count($equipamentos) > 0){
                foreach ($equipamentos as $equipamento){
                    $array_equipamentos[] = array(
                        'mveid'       => $equipamento['mveid'],
                        'tpedsc'      => utf8_encode($equipamento['tpedsc']),
                        'mcedsc'      => utf8_encode($equipamento['mcedsc']),
                        'eqmnumserie' => utf8_encode($equipamento['eqmnumserie']));
                }
            }
            
            if($pessoa['nu_matricula_siape']){
            	$cargo = $pessoa['ds_cargo_emprego'];
            } else {
            	$cargo = null;
            }

            $resultado = array(
                'equipamentos'       => $array_equipamentos,
                'vttid'              => $pessoa['vttid'],
                'nu_matricula_siape' => $pessoa['nu_matricula_siape'],
                'vttdoc'             => $pessoa['vttdoc'],
                'vttnome'            => utf8_encode($pessoa['vttnome']),
            	'ds_cargo_emprego'   => $cargo,
                'vttobs'             => utf8_encode($pessoa['vttobs']),
                'status'             => 'ok');
        } else{
            $resultado = array('status' => 'erro');
        }

    } else{
        if(!empty($vttid) || !empty($vttdoc) || !empty($vstid)){

            //$cracha = new CrachaProvisorio();
            //$expediente = $cracha->verificarExpedienteTrabalho();
            $expediente = SCA_EXPEDIENTE_NORMAL;

            if($expediente == SCA_EXPEDIENTE_NORMAL){

                $visitas = $oVisita->consultarSaidaPorDocumentoCracha($vttid, $vttdoc, $vstid);

                if($visitas && count($visitas) > 0){

                    $array_visitas = array();

                    if(count($visitas) == 1){
                        $visita = $visitas[0];
                        $array_visitas[] = $visita['vstid'];

                        $array_equipamentos = array();
                        $equipamentos = $oVisita->consultarSaidaEquipamento($visita['vttid']);
                        if($equipamentos && count($equipamentos) > 0){
                            foreach ($equipamentos as $equipamento){
                                $array_equipamentos[] = array(
                                    'mveid'       => $equipamento['mveid'],
                                    'tpedsc'      => utf8_encode($equipamento['tpedsc']),
                                    'mcedsc'      => utf8_encode($equipamento['mcedsc']),
                                    'eqmnumserie' => utf8_encode($equipamento['eqmnumserie']));
                            }
                        }

                        $resultado = array(
                            'visitas'      => $array_visitas,
                            'expedienteNormal' => true,
                            'equipamentos' => $array_equipamentos,
                            'vstid'        => $visita['vstid'],
                            'vttid'        => $visita['vttid'],
                            'nu_matricula_siape' => '',
                            'vttdoc'       => utf8_encode($visita['vttdoc']),
                            'vttnome'      => utf8_encode($visita['vttnome']),
                            'vttobs'       => utf8_encode($visita['vttobs']),
                            'status'       => 'ok'
                        );
                    }else{

                        foreach ( $visitas as $visita ){
                            $array_visitas[]= $visita['vstid'];
                        }
                        $resultado = array(
                            'visitas' => $array_visitas,
                            'status' => 'ok'
                        );
                    }
                }else{
                    $resultado = array('status' => 'erro');
                }
            } else{

                $visitas = $oVisita->consultarSaidaAutorizados($vttid, $vttdoc, $vstid, $nu_matricula_siape);

                if($visitas && count($visitas) > 0){

                    $array_visitas = array();

                    if(count($visitas) == 1){
                        $visita = $visitas[0];
                        $array_visitas[] = $visita['vstid'];

                        $array_equipamentos = array();
                        $equipamentos = $oVisita->consultarSaidaEquipamento($visita['vttid'], $visita['nu_matricula_siape']);
                        if($equipamentos && count($equipamentos) > 0){
                            foreach ($equipamentos as $equipamento){
                                $array_equipamentos[] = array(
                                    'mveid'       => $equipamento['mveid'],
                                    'tpedsc'      => utf8_encode($equipamento['tpedsc']),
                                    'mcedsc'      => utf8_encode($equipamento['mcedsc']),
                                    'eqmnumserie' => utf8_encode($equipamento['eqmnumserie']));
                            }
                        }

                        $resultado = array(
                            'visitas'      => $array_visitas,
                            'expedienteNormal' => false,
                            'equipamentos' => $array_equipamentos,
                            'vstid'        => $visita['vstid'],
                            'vttid'        => $visita['vttid'],
                            'nu_matricula_siape' => $visita['nu_matricula_siape'],
                            'vttdoc'       => utf8_encode($visita['vttdoc']),
                            'vttnome'      => utf8_encode($visita['vttnome']),
                            'vttobs'       => utf8_encode($visita['vttobs']),
                            'status'       => 'ok'
                        );
                    }else{

                        foreach ( $visitas as $visita ){
                            $array_visitas[]= $visita['vstid'];
                        }
                        $resultado = array(
                            'visitas' => $array_visitas,
                            'status' => 'ok'
                        );
                    }
                } else{
                    $resultado = array('status' => 'erro');
                }
            }
        } else{
            $resultado = array('status' => 'erro');
        }
    }

    echo simec_json_encode($resultado);

}else if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'buscarEquipamentoVisitante'){

    $eqmnumetiqueta = $_REQUEST['etiqueta'];

    $array_equipamentos = array();
    $equipamentos = $oVisita->consultarSaidaEquipamento(null, null, $eqmnumetiqueta);
    if($equipamentos && count($equipamentos) > 0){
        $selecionado = 0;
        foreach ($equipamentos as $equipamento){
            $array_equipamentos[] = array(
                'mveid'       => $equipamento['mveid'],
                'tpedsc'      => utf8_encode($equipamento['tpedsc']),
                'mcedsc'      => utf8_encode($equipamento['mcedsc']),
                'eqmnumserie' => utf8_encode($equipamento['eqmnumserie']));

            if($eqmnumetiqueta == $equipamento['eqmnumetiqueta']){
                $selecionado = $equipamento['mveid'];
            }
        }

        $pessoa = $oVisita->recuperarPessoa($equipamentos[0]['vttid'], null, $equipamentos[0]['nu_matricula_siape']);

        $resultado = array(
            'equipamentos'       => $array_equipamentos,
            'vttid'              => $pessoa['vttid'],
            'nu_matricula_siape' => $pessoa['nu_matricula_siape'],
            'vttdoc'             => $pessoa['vttdoc'],
            'vttnome'            => utf8_encode($pessoa['vttnome']),
            'vttobs'             => utf8_encode($pessoa['vttobs']),
            'selecionado'        => $selecionado,
            'status'             => 'ok');
    } else{
        $resultado = array('status' => 'erro');
    }

    echo simec_json_encode($resultado);

} else if($_REQUEST['servico'] == 'consultarEntradaCracha'){

    //pega os parâmetros
    $nu_matricula_siape = $_REQUEST['nu_matricula_siape'];

    $servidor = $oVisita->recuperarServidor($nu_matricula_siape);

    if($servidor){
        $resultado = array(
            'nu_matricula_siape' => $servidor['nu_matricula_siape'],
            'vttnome'            => utf8_encode(trim($servidor['vttnome'])),
            'ds_cargo_emprego'   => utf8_encode(trim($servidor['ds_cargo_emprego'])),
            'lotacao'            => utf8_encode(trim($servidor['no_unidade_org'])),
            'sala'               => utf8_encode(trim($servidor['usdsala'])),
            'telefone'           => utf8_encode(trim($servidor['usufonenum'])),
            'status'             => 'ok');
    } else{
        $resultado = array('status' => 'erro');
    }

    echo simec_json_encode($resultado);

} else if($_REQUEST['servico'] == 'consultarBaixaCracha'){

    //pega os parâmetros
    $vstnumcracha = $_REQUEST['vstnumcracha'];

    $cracha = $oVisita->consultarSaidaCracha($vstnumcracha);

    if($cracha){
        $resultado = array(
            'cpsid'              => $cracha['cpsid'],
            'nu_matricula_siape' => $cracha['nu_matricula_siape'],
            'vstnumcracha'       => $cracha['vstnumcracha'],
            'vttnome'            => utf8_encode($cracha['vttnome']),
            'ds_cargo_emprego'   => utf8_encode($cracha['ds_cargo_emprego']),
            'status'             => 'ok');
    } else{
        $resultado = array('status' => 'erro');
    }

    echo simec_json_encode($resultado);
}









//caso a chamada do ajax seja da busca de solicitante da tela de autorização de acesso fora do horário
else if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'consultarSolicitanteAcessoForaHorario'){

    $tipoConsulta = $_REQUEST['tipoConsulta'];
    if($tipoConsulta == 'S'){
        $nu_cpf = $_REQUEST['nu_cpf'];
        $sql = "SELECT servidor.nu_matricula_siape,servidor.no_servidor FROM ";
        $sql .= "siape.tb_siape_cadastro_servidor servidor WHERE servidor.nu_cpf='".$nu_cpf."' ";
        $resultado = $db->carregar($sql);
    }
    echo
    campo_texto('solicitantenome','S','N','Solicitante',50,50,'','','left','',0,'','',$resultado[0]['no_servidor'])."
    <input style=\"cursor: pointer;\"  type=\"button\" value=\"Pesquisar\" id=\"btnPesquisarSolicitante\" onclick=\"javascript:consultarPessoa(1);\">
    <input type='hidden' name='solicitantematricula' id='solicitantematricula' value='".$resultado[0]['nu_matricula_siape']."'>";

}
//caso a chamada do ajax seja da busca de pessoas da tela de autorização de acesso fora do horário
else if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'consultarPessoasAcessoForaHorario'){

    $tipoConsulta = $_REQUEST['tipoConsulta'];
    if($tipoConsulta == 'S'){
        $nu_cpf = $_REQUEST['id'];
        $sql = "SELECT servidor.nu_matricula_siape,servidor.no_servidor FROM ";
        $sql .= "siape.tb_siape_cadastro_servidor servidor WHERE servidor.nu_cpf='".$nu_cpf."' ";
        $resultado = $db->carregar($sql);
        $arr_json = array('codigo'=>$resultado[0]['nu_matricula_siape'],'descricao'=>utf8_encode($resultado[0]['no_servidor']));
    }
    else if($tipoConsulta == 'V'){
        $vttid = $_REQUEST['id'];
        $sql = "SELECT visitante.vttnome FROM sca.visitante visitante WHERE visitante.vttid='".$vttid."' ";
        $resultado = $db->carregar($sql);
        $arr_json = array('codigo'=>$vttid,'descricao'=>utf8_encode($resultado[0]['vttnome']));
    }
    echo simec_json_encode($arr_json);


}
// busca documento pela etiqueta do equipamento 
else if(isset($_REQUEST['servico']) &&  $_REQUEST['servico']== 'buscarDocumentoPorEtiqueta'){

    $eqmnumetiqueta = strtoupper($_REQUEST['eqmnumetiqueta']) ;

    $sql = "SELECT
                vttid 
            FROM
                sca.equipamento eq 
            INNER JOIN 
                sca.movimentacaoequipamento me 
                ON me.eqmid=eq.eqmid
            WHERE
                eqmnumetiqueta = '$eqmnumetiqueta' and mvedatsaida is null";
    
    
    $vttid = $db->pegaUm($sql);

    $arr_json = array('vttid' => $vttid);
    echo simec_json_encode($arr_json);

}

?>