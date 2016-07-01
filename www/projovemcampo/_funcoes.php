
<?php
function subistituiCaracteres($string) {
	$palavra = strtr ( $string, "ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ", "SOZsozYYuAAAAAAACEEEEIIIIDNOOOOOOUUUUYsaaaaaaaceeeeiiiionoooooouuuuyy" );
	$palavranova = str_replace ( "_", " ", $palavra );
	$pattern = '|[^a-zA-Z0-9\-]|';
	$palavranova = preg_replace ( $pattern, ' ', $palavranova );
	$string = str_replace ( ' ', ' ', $palavranova );
	$string = str_replace ( '---', '', $string );
	$string = str_replace ( '--', '', $string );
	$string = str_replace ( '-', '', $string );
	return $string;
}
function cancelarAdesao(){
	global $db;

	$sql = "UPDATE projovemcampo.adesaoprojovemcampo
			SET apcstatus = 'f'
			WHERE apcid = {$_REQUEST['apcid']};	";
	$docid = $db->pegaUm($sql);
	$db->commit();
	echo "<script>
			alert('Adesão cancelada com sucesso!');
    			window.location.href = window.location.href
		  </script>";
	die();
}

function reativarAdesao(){
	global $db;

	$sql = "UPDATE projovemcampo.adesaoprojovemcampo
			SET apcstatus = 't'
			WHERE apcid = {$_REQUEST['apcid']};	";
	$docid = $db->pegaUm($sql);
	$db->commit();

	echo "<script>
			alert('Adesão reativada com sucesso!');
    			window.location.href = window.location.href
		  </script>";
	die();
}
function bloqueioadesao(){
	global $db;

		$dataAtual      = mktime( date('H'), date('i'), date('s'), date('m'), date('d'), date('Y') );
		$dataBloqueio   = mktime( 23, 59, 00, 08,6, 2014 );
	$bloqueioHorario = (bool) ( $dataAtual > $dataBloqueio );

	return $bloqueioHorario;
	// 	die;
}


function criaDocumento() {
	global $db;
	
	$esdid = $db->pegaUm("SELECT esdid FROM workflow.estadodocumento WHERE tpdid='".TPD_PROJOVEMCAMPO."' ORDER BY esdordem ASC LIMIT 1");
	
	$sql = "INSERT INTO workflow.documento(
            tpdid, esdid, docdsc)
    		VALUES ('".TPD_PROJOVEMCAMPO."', '".$esdid."', 'Projovem Campo ".$_SESSION['projovemcampo']['apcid']."') RETURNING docid;";
	
	$docid = $db->pegaUm($sql);
	$db->commit();
	
	return $docid;
}

function pesquisarEscolas($dados) {
	global $db;
	
        $whereEntnome = '';
        if (isset($dados['entnome']) && !empty($dados['entnome'])) {
          $dados['entnome'] = strtoupper($dados['entnome']);
          $whereEntnome = " AND no_entidade LIKE '%{$dados['entnome']}%' ";
        }
	if($_SESSION['projovemcampo']['estuf']){
		$inner = "INNER JOIN territorios.municipio mun ON ads.apccodibge::character(2) = mun.estuf and apcesfera='E'";
	}else{
		$inner = "INNER JOIN territorios.municipio mun ON mun.muncod = ads.apccodibge::character(7) AND apcesfera = 'M'";
	}
    $sqlfiltro = "SELECT Distinct
						tur.entid
					FROM
						projovemcampo.turma tur
					INNER JOIN projovemcampo.adesaoprojovemcampo ads ON ads.secaid = tur.secaid
					INNER JOIN territorios.municipio mun ON mun.muncod = ads.apccodibge::character(7) AND apcesfera = 'M'
					WHERE
						apcid !='{$_SESSION['projovemcampo']['apcid']}'
    				AND turstatus = 'A'";
	$sql = "SELECT 
				'<img src=../imagens/alterar.gif border=0 style=cursor:pointer; onclick=\"marcarCodigoInep(\''||pk_cod_entidade||'\');\">' as acao, 
				pk_cod_entidade, no_entidade 
			FROM 
				educacenso_2013.tab_entidade tbe
			INNER JOIN entidade.entidade ent ON ent.entcodent = tbe.pk_cod_entidade::character
			WHERE 
				fk_cod_municipio='".$dados['muncod']."' AND id_dependencia_adm='".$dados['id_dependencia_adm']."'
                                {$whereEntnome}
            AND ent.entid not in ($sqlfiltro)           
                        ORDER BY no_entidade";
// 	ver($sql);
	$db->monta_lista_simples($sql,$cabecalho,5000,5,'N','100%',$par2);
	
}


function montaMenuFormacaoEducadores() {
	global $db;
	$menu[] = array("id" => 1, "descricao" => "Formação de Educadores", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores&aba2=formacaoEducadoresCadastro");
	$menu[] = array("id" => 2, "descricao" => "Formação de Educadores - Resumo", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores&aba2=formacaoEducadoresResumo");
	
	return $menu;
}

function atualizarProjovemMunicipio($dados) {
	global $db;
	
	$sql = "UPDATE projovemcampo.municipio
			   SET muncod=".(($dados['muncod'])?"'".$dados['muncod']."'":"NULL")."
			 WHERE munid='".$dados['munid']."';";
	$db->executar($sql);
	$db->commit();
	
}




function listarMunicipiosCadastrados($dados) {
	global $db;
	
	if($dados['adesao']) $adesao=" AND prj.apctermoaceito=TRUE";
	
	$sql = "SELECT mun.mundescricao, usu.usunome, usu.usuemail, '('||SUBSTR(ifs.isetelefone::text,1,2)||')'||SUBSTR(ifs.isetelefone::text,3) as tel 
                FROM projovemcampo.usuarioresponsabilidade urs 
                LEFT JOIN seguranca.usuario usu ON usu.usucpf=urs.usucpf 
                LEFT JOIN projovemcampo.adesaoprojovemcampo prj ON urs.muncod=prj.muncod
                INNER JOIN territorios.municipio mun ON mun.muncod=urs.muncod 
                LEFT JOIN projovemcampo.identificacaosecretario ifs ON ifs.apcid=prj.apcid 
                WHERE mun.muncod IS NOT NULL AND rpustatus='A'".$adesao."
                ORDER BY mun.mundescricao, usu.usunome
        ";
	
	$cabecalho = array("Município","Secretário","E-mail","Telefone");
	
	if(!$dados['relatorio']) echo "<div style=height:370;overflow:auto;>";
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	if(!$dados['relatorio']) echo "</div>";
	if(!$dados['relatorio']) echo "<p align=center><input type=button value=Fechar onclick=\"closeMessage();\"></p>";
	
}

function listarEstadosCadastrados($dados) {
	global $db;
	
	if($dados['adesao']) $adesao=" AND prj.apctermoaceito=TRUE";
	
	$sql = "SELECT est.estdescricao, usu.usunome, usu.usuemail, '('||SUBSTR(ifs.isetelefone::text,1,2)||')'||SUBSTR(ifs.isetelefone::text,3) as tel 
                FROM projovemcampo.usuarioresponsabilidade urs 
                LEFT JOIN seguranca.usuario usu ON usu.usucpf=urs.usucpf 
                LEFT JOIN projovemcampo.adesaoprojovemcampo prj ON urs.estuf=prj.estuf
                INNER JOIN territorios.estado est ON est.estuf=urs.estuf 
                LEFT JOIN projovemcampo.identificacaosecretario ifs ON ifs.apcid=prj.apcid 
                WHERE est.estuf IS NOT NULL AND rpustatus='A'".$adesao."
                ORDER BY est.estdescricao, usu.usunome            
        ";
	
	$cabecalho = array("Estados","Secretário","E-mail","Telefone");
	
	echo "<div style=height:370;overflow:auto;>";
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	echo "</div>";
	echo "<p align=center><input type=button value=Fechar onclick=\"closeMessage();\"></p>";
	
}


function registarUltimoAcesso() {
	global $db;
	
		$db->executar("UPDATE projovemcampo.adesaoprojovemcampo SET paginaultimoacesso='".$_SERVER['REQUEST_URI']."' WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");
		$db->commit();
}

function encaminharUltimoAcesso() {
	global $db;
	$sql = "SELECT paginaultimoacesso FROM projovemcampo.adesaoprojovemcampo WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'";
	$paginaultimoacesso = $db->pegaUm($sql);
	
	if($paginaultimoacesso && $_SERVER['REQUEST_URI']!=$paginaultimoacesso) {
		die("<script>window.location='{$paginaultimoacesso}';</script>");
	}
}

function carregarProJovemCampoUF_MUNCOD( ){
	
	global $db;
	unset($_SESSION['projovemcampo']['estuf']);
	unset($_SESSION['projovemcampo']['muncod']);
	if( $_SESSION['projovemcampo']['apcid'] ){
		$sql = "SELECT
					apccodibge,
					apcesfera
				FROM
					projovemcampo.adesaoprojovemcampo
				WHERE
					apcid = ".$_SESSION['projovemcampo']['apcid'];
		$dados = $db->pegaLinha($sql);
// 		if($_SESSION['projovemcampo']['muncod']){
// 			$_SESSION['projovemcampo']['muncod'] = $_SESSION['projovemcampo']['muncod'];
// 		}else{
// 			$_SESSION['projovemcampo']['estuf']  = $_SESSION['projovemcampo']['estuf'];
// 		}
		if($dados['apcesfera']=='M'){
						
			$_SESSION['projovemcampo']['muncod'] = $dados['apccodibge'];
			
		}elseif( $dados['apcesfera']=='E'){
			$sql = "SELECT DISTINCT
						estuf
					FROM territorios.estado
					WHERE
						estcod = '{$dados['apccodibge']}'";
			$estuf = $db->pegaUm($sql);
			
			$_SESSION['projovemcampo']['estuf'] = $estuf;
		}
	}
}

function montaTituloEstMun(){
	global $db;
	
	if($_SESSION['projovemcampo']['muncod']){
		$sql = "SELECT mundescricao as descricao, estuf as uf
					FROM territorios.municipio
					WHERE muncod = '".$_SESSION['projovemcampo']['muncod']."'";
		$dado = $db->pegaLinha($sql);
	}
	
	if($_SESSION['projovemcampo']['estuf']){
		$sql = "SELECT estdescricao as descricao, estuf as uf
			FROM territorios.estado
					WHERE estuf = '".$_SESSION['projovemcampo']['estuf']."'";
		$dado = $db->pegaLinha($sql);
	}
	
	return $dado['descricao'];
	
}

function aceitarTermoAjustado($dados) {
    global $db;
    
    $perfis = pegaPerfilGeral();

    $sql .= "UPDATE projovemcampo.adesaoprojovemcampo SET adesaotermoajustadodata=NOW() , apctermoajustadoaceito='t' WHERE apcid='" . $_SESSION['projovemcampo']['apcid'] . "'";
    $db->executar($sql);
    $db->commit();
    
//     $sql = "select docid from projovemcampo.adesaoprojovemcampo where apcid = " . $_SESSION['projovemcampo']['apcid'];
//     $docid = $db->pegaUm( $sql );
    
//     $aedid = $_REQUEST['aedid'];
    
//     $dados = array();
    
//     wf_alterarEstado($docid, $aedid, $cmddsc, $dados);
    
    $url = "projovemcampo.php?modulo=principal/termoAdesaoAjustado&acao=A";
    
    echo "
            <script>
            alert('Termo Ajustado foi aceito com sucesso');
            window.location='".$url."';
            </script>
        ";
}

function naoAceitarTermoAjustado() {
	global $db;
	
    $sql .= "UPDATE projovemcampo.adesaoprojovemcampo SET  apctermoajustadoaceito='f' WHERE apcid='" . $_SESSION['projovemcampo']['apcid'] . "'";
    $db->executar($sql);
    $db->commit();
    
	echo "<script>
			alert('Termo Ajustado não foi aceito com sucesso');
			window.location='projovemcampo.php?modulo=principal/".(($_SESSION['projovemcampo']['estuf'])?"listaEstados":"").(($_SESSION['projovemcampo']['muncod'])?"listaMunicipios":"")."&acao=A';
		  </script>";
}

function montaMenuProjovemCampo() {

    global $db;
    
    if (!$_SESSION['projovemcampo']['apcid']) {
    	die ( "
            <script>
                alert('Problemas de navegação. Inicie novamente.');
                window.location='projovemcampo.php?modulo=inicio&acao=C';
            </script>" );
    }

    $docid = $db->pegaUm("SELECT docid FROM projovemcampo.adesaoprojovemcampo WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");
	if(!$docid) {
		$docid = criaDocumento();
		$db->executar("UPDATE projovemcampo.adesaoprojovemcampo SET docid='".$docid."' WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");
		$db->commit();
	}
	
	$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");
    
    $perfis = pegaPerfilGeral();
    
    if (!$_SESSION['projovemcampo']['apcid']){
        die("
            <script>
                alert('Problemas de navegação. Inicie novamente.');
                window.location='projovemcampo.php?modulo=inicio&acao=C';
            </script>"
        );
    }
    $sql = "SELECT
							s.secoordcpf
						FROM
									projovemcampo.secretaria s
						INNER JOIN	projovemcampo.adesaoprojovemcampo a ON a.secaid = s.secaid
						WHERE
							apcid='".$_SESSION['projovemcampo']['apcid']."'";
     
    $coordenadorresponsavel = $db->pegaUm($sql);
    $identificacao = $db->pegaUm("SELECT sec.secoid FROM projovemcampo.adesaoprojovemcampo as ad
    								INNER JOIN projovemcampo.secretaria as s on s.secaid = ad.secaid
    								INNER JOIN projovemcampo.secretario as sec on sec.secoid = s.secoid
    								WHERE apcid='" . $_SESSION['projovemcampo']['apcid'] . "'");
    
    //Adaptação para o perfil Diretor do Escola
    if((!in_array(PFL_DIRETOR_ESCOLA, $perfis) &&!in_array(PFL_COORDENADOR_TURMA, $perfis))||in_array ( PFL_CONSULTA, $perfis ) ||in_array(PFL_SUPER_USUARIO, $perfis)/*&&!in_array(PFL_COORDENADOR_ESTADUAL,$perfis)&&!in_array(PFL_COORDENADOR_MUNICIPAL,$perfis)*/){
	    $menu = array(
	            0 => array(
	                "id" => 1, 
	                "descricao" => "Instruções", 
	                "link" => "/projovemcampo/projovemcampo.php?modulo=principal/instrucao&acao=A" . (($_SESSION['projovemcampo']['estuf']) ? "&estuf=" . $_SESSION['projovemcampo']['estuf'] : "") . (($_SESSION['projovemcampo']['muncod']) ? "&muncod=" . $_SESSION['projovemcampo']['muncod'] : "")),
	            1 => array(
	                "id" => 2, 
	                "descricao" => 
	                "Identificação", 
	                "link" => "/projovemcampo/projovemcampo.php?modulo=principal/identificacao&acao=A"
	            )
	    );
    }
    elseif(in_array(PFL_DIRETOR_ESCOLA, $perfis)){
    	$menu[] = array("id" => 1,"descricao" => "Monitoramento","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A");
    	return $menu;
    }
      #MONTA MENU MUNICIPIO.
    if ($_SESSION['projovemcampo']['muncod'] && $identificacao) {

        $metaDirecionada = 1;

// ver($_SESSION['projovemcampo']['muncod'],$identificacao,d);
        if ( $metaDirecionada > 0 ){
        
            $apctermoaceito = $db->pegaLinha("SELECT apctermoaceito, apctermoajustadoaceito FROM projovemcampo.adesaoprojovemcampo WHERE apcid='{$_SESSION['projovemcampo']['apcid']}'");

            # REVER ESTA REGRA
//             $abaTermo = podeMostrarTermosMetas();
			if(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)|| in_array(PFL_CONSULTA, $perfis)||in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_EQUIPE_MEC, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis)  || in_array(PFL_SECRETARIO_MUNICIPAL, $perfis )|| in_array(PFL_SECRETARIO_ESTADUAL, $perfis )){
	            $abaTermo = 1;
	            if ($abaTermo){
	                $menu[] = array("id" => 3, "descricao" => "Termo de Adesão", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/termoAdesao&acao=A");
	            }
           
            	if ($apctermoaceito['apctermoaceito'] == "t") {
            
            		$menu[] = array(
            				"id" => 4,
            				"descricao" => "Sugestão de Meta",
            				"link" => "/projovemcampo/projovemcampo.php?modulo=principal/sugestaoAmpliacao&acao=A"
            		);
	            	$sql = "select apcampliameta from projovemcampo.adesaoprojovemcampo as ad inner join projovemcampo.meta as m on m.apcid = ad.apcid where m.tpmid = 3 and ad.apcampliameta = 't' and ad.apcid=".$_SESSION['projovemcampo']['apcid'];
	            	$abaTermoAjustado = $db->pegaUm($sql);
	
	            	if ($abaTermoAjustado) {
	            		
	            		$menu[] = array("id" => 5, "descricao" => "Termo de adesão ajustado", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/termoAdesaoAjustado&acao=A");
	            	}
            	}
            	
	            	$menu[] = array("id" => 6, "descricao" => "Escola/Turma", "link" => "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A");
	            	
            	if($coordenadorresponsavel){
            		
	            	$menu[] = array("id" => 7, "descricao" => "Plano de Implementação", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A");
            		
            		$menu[] = array("id" => 8,"descricao" => "Monitoramento","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A");
            	}
            }
            if(in_array(PFL_COORDENADOR_TURMA, $perfis)){
            	$menu[] = array("id" => 6, "descricao" => "Escola/Turma", "link" => "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A");
            	if($coordenadorresponsavel){
            
            		$menu[] = array("id" => 7,"descricao" => "Monitoramento","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A");
            	}
            }
            if(in_array(PFL_SUPER_USUARIO, $perfis) ||in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)||in_array(PFL_ADMINISTRADOR, $perfis)){
				$menu [] = array ("id" => 8,"descricao" => "Transferência de Aluno","link" => "/projovemcampo/projovemcampo.php?modulo=principal/transferencia&acao=A");
            }
		}
    }
    #MONTA MENU ESTADO.
    if( $_SESSION['projovemcampo']['estuf'] && $identificacao ){
        

		# REVER ESTA REGRA
//      $abaTermo = podeMostrarTermosMetas();
        $abaTermo = 1;
		if(in_array(PFL_COORDENADOR_ESTADUAL, $perfis)||in_array(PFL_CONSULTA, $perfis)|| in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_EQUIPE_MEC, $perfis) 
			|| in_array(PFL_ADMINISTRADOR, $perfis)  || in_array(PFL_SECRETARIO_MUNICIPAL, $perfis )|| in_array(PFL_SECRETARIO_ESTADUAL, $perfis )){
	        if ($abaTermo){
	            $menu[] = array("id" => 3, "descricao" => "Termo de Adesão", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/termoAdesao&acao=A");
	        }
	        $apctermoaceito = $db->pegaLinha("SELECT apctermoaceito, apctermoajustadoaceito FROM projovemcampo.adesaoprojovemcampo WHERE apcid='{$_SESSION['projovemcampo']['apcid']}'");
        
   			if ($apctermoaceito['apctermoaceito'] == "t") {
            
            	$menu[] = array(
            			"id" => 4,
            			"descricao" => "Sugestão de Meta",
            			"link" => "/projovemcampo/projovemcampo.php?modulo=principal/sugestaoAmpliacao&acao=A"
            	);
            }
				$sql = "select apcampliameta from projovemcampo.adesaoprojovemcampo as ad inner join projovemcampo.meta as m on m.apcid = ad.apcid where m.tpmid = 3 and ad.apcampliameta = 't' and ad.apcid=".$_SESSION['projovemcampo']['apcid'];
            	$abaTermoAjustado = $db->pegaUm($sql);
               
	        if ($abaTermoAjustado) {
	        
	        	$menu[] = array("id" => 5, "descricao" => "Termo de adesão ajustado", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/termoAdesaoAjustado&acao=A");
	        }
        
			$menu[] = array("id" => 6, "descricao" => "Escola/Turma", "link" => "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A");
			
	        if($coordenadorresponsavel){
	        	
	        	$menu[] = array("id" => 7, "descricao" => "Plano de Implementação", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A");
	        	
	        	$menu[] = array("id" => 8,"descricao" => "Monitoramento","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A");
	        	
	        }
        }
        if(in_array(PFL_COORDENADOR_TURMA, $perfis)){
        	
        	$menu[] = array("id" => 6, "descricao" => "Escola/Turma", "link" => "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A");
        	
        	if($coordenadorresponsavel){
        
        		$menu[] = array("id" => 7,"descricao" => "Monitoramento","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A");
        		
        	}
        }
        if(in_array(PFL_SUPER_USUARIO, $perfis) ||in_array(PFL_COORDENADOR_ESTADUAL, $perfis)||in_array(PFL_ADMINISTRADOR, $perfis)){
        	$menu [] = array ("id" => 8,"descricao" => "Transferência de Aluno","link" => "/projovemcampo/projovemcampo.php?modulo=principal/transferencia&acao=A");
        }
    }
    return $menu;
}

function montaMenuMonitoramento() 
{
	global $db;
	$menu   = array();
	$perfis = pegaPerfilGeral();
        
// 	if($db->testa_superuser() 
//  		|| in_array(PFL_SUPER_USUARIO, $perfis) 
// 		|| in_array(PFL_EQUIPE_MEC, $perfis)
// 		|| in_array(PFL_CONSULTA, $perfis)) {
		$menu[] = array("id" => 1, "descricao" => "Cadastro de Estudantes","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes");
		$menu[] = array("id" => 2, "descricao" => "Diários de Frequência","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=diarioFrequencia");
		$menu[] = array("id" => 3, "descricao" => "Frequência Mensal","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=frequenciaMensal");
		$menu[] = array("id" => 5, "descricao" => "Agência Bancária","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=agencias");
		$menu[] = array("id" => 6, "descricao" => "Encaminhar Lista","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista");
		if($db->testa_superuser()|| in_array(PFL_SUPER_USUARIO, $perfis)||in_array(PFL_ADMINISTRADOR, $perfis)){
			$menu[] = array("id" => 7, "descricao" => "Altera Data Inicio Diário","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=alteraRangeDiario");
		}
// 		$menu[] = array("id" => 8, "descricao" => "Acompanhamento de Frequência e Notas","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq");
// 	}else if(in_array(PFL_DIRETOR_ESCOLA, $perfis)) {
// 		$menu[] = array("id" => 1, "descricao" => "Cadastro de Estudantes","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes");
// 		$menu[] = array("id" => 2, "descricao" => "Diários de Frequência e Trabalhos","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=diarioFrequencia");
// 		$menu[] = array("id" => 3, "descricao" => "Frequência Mensal","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=frequenciaMensal");
// 		$menu[] = array("id" => 6, "descricao" => "Encaminhar Lista","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista");
// 		$menu[] = array("id" => 8, "descricao" => "Acompanhamento de Frequência e Notas","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq"); 
// 	} else if(in_array(PFL_COORDENADOR_TURMA, $perfis)) {
// 		$menu[] = array("id" => 7, "descricao" => "Encaminhar Lista","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista");
// 		$menu[] = array("id" => 8, "descricao" => "Acompanhamento de Frequência e Notas","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq");
// 	}else if(in_array(PFL_COORDENADOR_ESTADUAL, $perfis) || in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
// 		$menu[] = array("id" => 1, "descricao" => "Cadastro de Estudantes","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes");	
// 		$menu[] = array("id" => 5, "descricao" => "Agência","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=agencias");
// 		$menu[] = array("id" => 6, "descricao" => "Encaminhar Lista","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista");
// 		$menu[] = array("id" => 8, "descricao" => "Acompanhamento de Frequência e Notas","link" => "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq");
//   }
  return $menu;
}

function carregarMunicipios2($dados) {
	global $db;
	if($dados['estuf']){ $sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$dados['estuf']."' ORDER BY mundescricao";
// 	ver($sql,d);
	}else{ $sql = array();}
	if($dados['modulo'] == 'principal/listaEstudantesMonitoramento'){
		$db->monta_combo('muncod', $sql, 'S', 'Selecione', pegarEscolas2, '', '', '', 'N', 'muncod');
	}else{
		$db->monta_combo('muncod', $sql, 'S', 'Selecione', $funcao, '', '', '', 'N', 'muncod');
	}
}

function carregarMunicipios($dados) {
	global $db;
        if($dados['estuf']){
        	if($dados['muncod']){
        		$muncod = "AND muncod = '{$dados['muncod']}'";
        	}
        	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$dados['estuf']."' $muncod ORDER BY mundescricao";
		}else{
			$sql = array();
		}
        $dados['bloq'] = $dados['bloq'] ? $dados['bloq'] : 'S';
        if ($dados['nat']) {
          $db->monta_combo('estmuncodnasc', $sql, $dados['bloq'], 'Selecione o Munícipio', '', '', '', '', 'S', 'estmuncodnasc', null, null, null, 'required');
        } else {
        	$municipio = $db->pegaLinha($sql);
        	echo $municipio['descricao'];
//           $db->monta_combo('estendmuncod', $sql, $dados['bloq'], 'Selecione', '', '', '', '', 'S', 'estendmuncod');
        }
}

function testaPolo($post){
	
	global $db;
	
	if( $post['estuf'] != '' ){
		$filtro = "AND estuf = '".$post['estuf']."'";
	}else{
		$filtro = "AND muncod = '".$post['muncod']."'";
	}
	$sql = "SELECT
				'S'
			FROM
				projovemcampo.adesaoprojovemcampo pju
			INNER JOIN projovemcampo.polomunicipio pmu ON pmu.apcid = pju.apcid
			WHERE
				pmupossuipolo IS TRUE 
				$filtro";
	echo $db->pegaUm($sql);
}

function montaMenuPlanoImplementacao() {
	global $db;
	
	$menu[] = array("id" => 1, "descricao" => "Coordenador Responsável", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=coordenadorResponsavel");
	$menu[] = array("id" => 2, "descricao" => "Meta, Matrícula e Início de aula", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=metaMatriculaInicioAula");
	$menu[] = array("id" => 3, "descricao" => "Pólo/Núcleo", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo");
	$menu[] = array("id" => 4, "descricao" => "Profissionais", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=profissionais");
	$menu[] = array("id" => 5, "descricao" => "Formação de Educadores", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=formacaoEducadores");
	$menu[] = array("id" => 6, "descricao" => "Gêneros Alimenticios", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=generoAlimenticios");
	if($_SESSION['projovemcampo']['estuf']) $ab="qualificacaoProfissionalEstado";
	if($_SESSION['projovemcampo']['muncod']) $ab="qualificacaoProfissionalMunicipio";
	$menu[] = array("id" => 7, "descricao" => "Qualificação Profissional", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=".$ab);
	if($_SESSION['projovemcampo']['estuf']) $menu[] = array("id" => 11, "descricao" => "Transporte Mat. Didático", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=transporteDidatico");
	$menu[] = array("id" => 8, "descricao" => "Demais Ações", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=demaisAcoes");
	
	$menu[] = array("id" => 9, "descricao" => "Resumo Financeiro", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=resumoFinanceiro");
	$menu[] = array("id" => 10, "descricao" => "Repasse de Recurso", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=repasseRecurso");
	$menu[] = array("id" => 10, "descricao" => "Visualizar Plano", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=visualizarPlano");
	
	return $menu;
	
	
}

function montaMenuPoloNucleo() {
	global $db;
	
// 	$menu[] = array("id" => 1, "descricao" => "Pólo/Núcleo", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoCadastro");
// 	if($_REQUEST['aba2']=="poloNucleoGerenciar") $menu[] = array("id" => 2, "descricao" => "Pólo/Núcleo - Gerenciar", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoGerenciar");
// 	$menu[] = array("id" => 3, "descricao" => "Pólo/Núcleo - Resumo", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=poloNucleo&aba2=poloNucleoResumo");
	$menu[] = array("id" => 1, "descricao" => "Pólo/Núcleo", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=poloNucleo&aba2=poloNucleoCadastro");
	if($_REQUEST['aba2']=="poloNucleoGerenciar") $menu[] = array("id" => 2, "descricao" => "Pólo/Núcleo - Gerenciar", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=poloNucleo&aba2=poloNucleoGerenciar");
	$menu[] = array("id" => 3, "descricao" => "Pólo/Núcleo - Resumo", "link" => "/projovemcampo/projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=poloNucleo&aba2=poloNucleoResumo");
	
	return $menu;
	
}


// function inserirCoordenadorResponsavel($dados) {
// 	global $db;
	
// 	$sqlsecaid = "SELECT
// 				secaid
// 			FROM
// 				projovemcampo.adesaoprojovemcampo
// 			WHERE
// 				apcid = '".$_SESSION['projovemcampo']['apcid']."'";
// 	$secaid = $db->pegaUm($sqlsecaid);
	
// 	$sql = "INSERT INTO projovemcampo.secretaria(secoordcpf)
//     		VALUES ('".$_SESSION['projovemcampo']['apcid']."', 
//     				'".str_replace(array(".","-"),array("",""),$dados['corcpf'])."', 
//     				'".$dados['cornome']."', 
//     				".(($dados['corsecretario']=="sim")?"TRUE":"FALSE").", 
//     				'A');";
	
// 	$db->executar($sql);
	
// 	$db->commit();
	
// 	echo "<script>
// 			alert('Dados salvos com sucesso');
// 			window.location='projovemcampo.php?modulo=principal/planoImplementacao&acao=A&aba=coordenadorResponsavel';
// 		  </script>";

// }

function atualizarCoordenadorResponsavel($dados) {
	global $db;
	
	$usuario = "SELECT 
					true					
				FROM seguranca.usuario			
			 	WHERE
					usucpf = '".str_replace(Array('.','-'),'',$dados['secoordcpf'])."'
				--AND usustatus = 'A'
							";
	$testausuario = $db->pegaUm($usuario);
// 	ver($usuario,d);
	if(!$testausuario){
		echo "<script>
			alert('Só se pode nomear usuário que ja tenha cadastro no simec.');
			window.location='projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=coordenadorResponsavel';
		  </script>";
		return false;
	}
	
	$sqlsecaid = "SELECT
				secaid
			FROM
				projovemcampo.adesaoprojovemcampo
			WHERE
				apcid = '".$_SESSION['projovemcampo']['apcid']."'";
	$secaid = $db->pegaUm($sqlsecaid);
	
	$sql = "SELECT
				secoordcpf
			FROM
				projovemcampo.adesaoprojovemcampo apc
			INNER JOIN projovemcampo.secretaria sec on sec.secaid = apc.secaid
			WHERE
				apcid = '".$_SESSION['projovemcampo']['apcid']."'";
	
	$cpf = $db->pegaUm($sql);
		$perfil = ( $_SESSION['projovemcampo']['muncod'] != '' ? PFL_COORDENADOR_MUNICIPAL : PFL_COORDENADOR_ESTADUAL);
		
		$campo = ( $_SESSION['projovemcampo']['muncod'] != '' ? 'muncod' : 'estuf');
	if($cpf!=''){
		$sql = '';
		
		
		$sql .= "DELETE FROM seguranca.perfilusuario WHERE usucpf in ('".$cpf."') AND pflcod = $perfil;";
		$sql .= "UPDATE projovemcampo.usuarioresponsabilidade SET
					 rpustatus = 'I'
				WHERE
					usucpf in ('".$cpf."')
						AND pflcod = $perfil
						AND $campo = '{$_SESSION['projovemcampo'][$campo]}';";
		
		$db->executar($sql);
		
		$db->commit();
	}
		$perfis = pegaPerfilGeral( str_replace(Array('.','-'),'',$dados['secoordcpf']) );
		$perfis = $perfis ? $perfis : Array();
		
		if( !in_array( $perfil, $perfis) ){
			$sql = "INSERT INTO seguranca.perfilusuario( usucpf, pflcod)
					VALUES( '".str_replace(Array('.','-'),'',$dados['secoordcpf'])."', $perfil );";
			
			$sql .= "INSERT INTO projovemcampo.usuarioresponsabilidade ( usucpf, pflcod, $campo)
			VALUES( '".str_replace(Array('.','-'),'',$dados['secoordcpf'])."', $perfil, '{$_SESSION['projovemcampo'][$campo]}' );";
		}
	$db->executar($sql);
			
	$db->commit();
	
	$sql = "UPDATE projovemcampo.secretaria
   			SET secoordcpf='".str_replace(array(".","-"),array("",""),$dados['secoordcpf'])."'
   			WHERE secaid = $secaid";
// 	ver($sql,d);
	$db->executar($sql);
	
	$db->commit();
	
	echo "<script>
			alert('Coordenador gravado com sucesso');
			window.location='projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=coordenadorResponsavel';
		  </script>";
}


function pegarUsuarioProJovemCampo(){
	global $db;

	if($_SESSION['projovemcampo']['apcid']){
		$sql = "
            SELECT * FROM projovemcampo.adesaoprojovemcampo as ad
        	LEFT JOIN projovemcampo.secretaria sec on sec.secaid = ad.secaid
			LEFT JOIN projovemcampo.secretario s on s.secoid = sec.secoid
            WHERE ad.apcid='".$_SESSION['projovemcampo']['apcid']."'
        ";
		return $db->pegaLinha($sql);
	}
	return array();
}

function pegameta(){
	global $db;

	if($_SESSION['projovemcampo']['estuf']){

		$sugestaoampliacao = $db->pegaUm("SELECT 1 FROM projovemcampo.adesaoprojovemcampo WHERE apcid='".$_SESSION['projovemcampo']['apcid']."' and apctermoajustadoaceito = 't'");
		
		if($sugestaoampliacao=='1'){

			$sql = "SELECT DISTINCT
						metvalor as valor,
						tpmdesc as tipo
					FROM
						projovemcampo.meta met
					INNER JOIN projovemcampo.tipometa tpm ON tpm.tpmid = met.tpmid
					WHERE
						met.apcid = {$_SESSION['projovemcampo']['apcid']}
					AND tpm.tpmid not in (1,2)
					ORDER BY
						tipo DESC ";

					$meta = $db->pegaUm($sql);

		}ELSE{
			$sql = "SELECT DISTINCT
						metvalor as valor,
						tpmdesc as tipo
					FROM
						projovemcampo.meta met
					INNER JOIN projovemcampo.tipometa tpm ON tpm.tpmid = met.tpmid
					WHERE
						met.apcid = {$_SESSION['projovemcampo']['apcid']}
					AND tpm.tpmid not in (2,3)
					ORDER BY
						tipo DESC ";
	
			$meta = $db->pegaUm($sql);
		}
	}
	
	if($_SESSION['projovemcampo']['muncod']) {

		$sugestaoampliacao = $db->pegaUm("SELECT 1 FROM projovemcampo.adesaoprojovemcampo WHERE apcid='".$_SESSION['projovemcampo']['apcid']."' AND apctermoajustadoaceito= 't'");
		
		if($sugestaoampliacao == '1'){
		
			$sql = "SELECT DISTINCT
						metvalor as valor,
						tpmdesc as tipo
					FROM
						projovemcampo.meta met
					INNER JOIN projovemcampo.tipometa tpm ON tpm.tpmid = met.tpmid
					WHERE
						met.apcid = {$_SESSION['projovemcampo']['apcid']}
					AND tpm.tpmid not in (1,2)
					ORDER BY
						tipo DESC ";
		
			$meta = $db->pegaUm($sql);
		}ELSE{
			$sql = "SELECT DISTINCT
						metvalor as valor,
						tpmdesc as tipo
					FROM
						projovemcampo.meta met
					INNER JOIN projovemcampo.tipometa tpm ON tpm.tpmid = met.tpmid
					WHERE
						met.apcid = {$_SESSION['projovemcampo']['apcid']}
					AND tpm.tpmid not in (2,3)
					ORDER BY
						tipo DESC ";
	
				$meta = $db->pegaUm($sql);
		}
	}
	
	return $meta;
	die;
}
function carregarProJovemCampo() {
    global $db;

    if ($_SESSION['projovemcampo']['estuf']) {
        $sql = "SELECT apcid FROM projovemcampo.adesaoprojovemcampo as ad 
				inner join territorios.estado as est on ad.apccodibge::character(2) = est.estcod::character(2) 
        		and apcesfera='E' WHERE est.estuf='" . $_SESSION['projovemcampo']['estuf'] . "'";

        $apcid = $db->pegaUm($sql);
        
        if($apcid) {
            $_SESSION['projovemcampo']['apcid'] = $apcid;
        } else {
            // pegando a secretaria de educação estadual
//             $sql = "
//                 SELECT *
//                 FROM entidade.entidade en 
//                 INNER JOIN entidade.funcaoentidade fe ON fe.entid = en.entid 
//                 INNER JOIN entidade.endereco ed ON ed.entid = en.entid  
//             	INNER JOIN territorios.estado est on est.estuf = ed.estuf
//                 WHERE ed.estuf='" . $_SESSION['projovemcampo']['estuf'] . "' AND fe.funid=6 AND fe.fuestatus='A' AND en.entstatus='A' AND ed.tpeid=1
//             ";
//             $entidade = $db->pegaLinha($sql);

//             $entnumcpfcnpj = ( $entidade['entnumcpfcnpj'] ? $entidade['entnumcpfcnpj'] : '' );
//             $telefone = trim( trim( $entidade['entnumdddcomercial'] ) . $entidade['entnumcomercial'] );

//             $endcep = ( $entidade['endcep'] ? $entidade['endcep'] : '' );
//             $estendlog = ( $entidade['estendlog'] ? $entidade['estendlog'] : '' );
//             $endcom = ( $entidade['endcom'] ? $entidade['endcom'] : '' );
//             $endbai = ( $entidade['endbai'] ? $entidade['endbai'] : '' );
//             $muncod  = ( $entidade['muncod'] ? $entidade['muncod'] : '' );
//             $estuf  = ( $entidade['estuf'] ? $entidade['estuf'] : '' );
//             $endnum  = ( $entidade['endnum'] ? $entidade['endnum'] : '' );
            
//             $sql = "INSERT INTO projovemcampo.secretaria( secacnpj, secatelefone, secacep, secaendereco, secabairro, secacomplemento, secanumero, secauf, secamuncod)
//                         VALUES ( '".$entnumcpfcnpj."', '".$telefone."', '".$endcep."', '".$estendlog."', '".$endcom."', '".$endbai."', '".$muncod."', '".$estuf."', '".$endnum."') RETURNING secaid;
//             ";
//             $secaid = $db->pegaUm($sql);
//             $db->commit();
//             $_SESSION['projovemcampo']['secaid'] = $secaid;
        }
    }else{
        if ($_SESSION['projovemcampo']['muncod'] ) {
				  $sql = "	SELECT apcid FROM projovemcampo.adesaoprojovemcampo as ad 
							inner join territorios.municipio as m on ad.apccodibge::character(7) = m.muncod 
			        		and apcesfera='M' WHERE m.muncod='" . $_SESSION['projovemcampo']['muncod'] . "'";

        	$apcid = $db->pegaUm($sql);
            if ($apcid) {
                $_SESSION['projovemcampo']['apcid'] = $apcid;
            } 
        }
    }
}

function inserirIdentificacao($dados) {
    global $db;
    $perfis = pegaPerfilGeral();
    
    $secacep             = str_replace(array("-"), array(""), $dados['secacep']);
    $secanumero          = $dados['secanumero'];
    $secacnpj          	 = str_replace(array("-"), array(""), str_replace(array("."), array(""), str_replace(array("/"), array(""), $dados['secacnpj'] ) ) );
    $secaendereco        = $dados['secaendereco'];
    $secacomplemento     = (($dados['secacomplemento']) ? "'" . $dados['secacomplemento'] . "'" : "NULL");
    $secabairro          = $dados['secabairro'];
    $secauf              = $dados['secauf'];
    $secamuncod       	 = $dados['secamuncod'];
    $secatelefone        = $dados['secatelefoneddd'] . str_replace(array("-"), array(""), $dados['secatelefone']);
    $secocelular         = $dados['secocelularddd'] . str_replace(array("-"), array(""), $dados['secocelular']);
    $secocpf             = str_replace(array("-"), array(""), str_replace(array("."), array(""), $dados['secocpf'] ) ) ;
    $seconumrg   		 = $dados['seconumrg'];
    $seconome   		 = $dados['seconome'];
    $secoorgaoexprg 	 = $dados['secoorgaoexprg'];
	$secaid 			 = $dados['secaid'];
	$secoid 			 = $dados['secoid'];
// 	ver($sqlUpdate,d);
    if($secoid != "" ) {

    	$sqlUpdate = "UPDATE projovemcampo.secretario SET
                        secocpf = '" .$secocpf. "',
                        seconome = '" . $dados['seconome'] . "',
                        seconumrg = '" . $dados['seconumrg'] . "',
                        secoorgaoexprg = '" . $dados['secoorgaoexprg'] . "',
                        secocelular = '" . $secocelular . "'
                      WHERE secoid = " .$secoid;
    	
 		
      	$db->executar($sqlUpdate);
        $db->commit();
    } else {
        $sql = "
            INSERT into projovemcampo.secretario(
                    secocpf, seconome, seconumrg, secoorgaoexprg, secocelular)
                        VALUES (
                        		'"  .$secocpf. "',
                        		'" .$seconome. "',
                        		'" .$seconumrg. "',
                        		'" .$secoorgaoexprg. "',
                        		'" .$secocelular. "') 
            returning secoid; ";

        $secoid = $db->pegaUm($sql);
    	$db->commit();
    }
   
    if($secaid != ""){
    	
        if ($dados['secacomplemento']) {
            $complemento = "secacomplemento = " . (($dados['secacomplemento']) ? "'" . $dados['secacomplemento'] . "'" : "NULL") . ",";
        }
        if ($dados['secamuncod']) {
            $municod = "secamuncod = '" . $dados['secamuncod'] . "',";
        }
        if ($dados['secauf']) {
            $uf = "secauf = '" . $dados['secauf'] . "',";
        }
        $sqlUpdate = "UPDATE projovemcampo.secretaria SET 
                        secacep = '" . str_replace(array("-"), array(""), $dados['secacep']) . "', 
                        secaendereco = '" . $dados['secaendereco'] . "', 
                        secanumero = '" . $dados['secanumero'] . "',
                        $complemento 
            		secabairro = '" . $dados['secabairro'] . "', 
            		$uf 
            		$municipio
            		secatelefone = '" . $secatelefone . "', 
            		secoid = $secoid
            		WHERE secaid = " .$secaid;
        $db->executar($sqlUpdate);
        $db->commit();
    } else {
        $sql = "
            INSERT INTO projovemcampo.secretaria(
                    secoid, secacep, secacnpj, secatelefone, secaendereco, secabairro, secacomplemento, secanumero, secauf, secamuncod)
                VALUES (
                    '" .$secoid. "', 
                    '" .$secacep. "', 
                    '" .$secacnpj. "', 
                    '" .$secatelefone. "', 
                    '" .$secaendereco. "', 
                    '" .$secabairro. "', 
        			" .$secacomplemento. ", 
					'" .$secanumero. "', 
					'" .$secauf. "', 
                    '" .$secamuncod. "')
        	RETURNING secaid;
		";

        $secaid = $db->pegaUm($sql);
        $db->commit();
        
        $sql = "UPDATE projovemcampo.adesaoprojovemcampo SET secaid = $secaid WHERE apcid = ".$_SESSION['projovemcampo']['apcid'];
        $db->executar($sql);
        $db->commit();
        
    }
    
    if ($_SESSION['projovemcampo']['estuf']){
        if (in_array(PFL_SECRETARIO_MUNICIPAL, $perfis) || in_array(PFL_SECRETARIO_ESTADUAL, $perfis)) {
            $urlRedirect = "projovemcampo.php?modulo=principal/identificacao&acao=A";
        }else{
           $urlRedirect = "projovemcampo.php?modulo=principal/termoAdesao&acao=A";
        }
    } elseif ($_SESSION['projovemcampo']['muncod']) {
        if (in_array(PFL_SECRETARIO_MUNICIPAL, $perfis) || in_array(PFL_SECRETARIO_ESTADUAL, $perfis)) {
            $urlRedirect = "projovemcampo.php?modulo=principal/identificacao&acao=A";
        }else{
           $urlRedirect = "projovemcampo.php?modulo=principal/termoAdesao&acao=A";
        }        
    } else {
        $urlRedirect = "projovemcampo.php?modulo=principal/identificacao&acao=A";
    }
    echo "
        <script>
            alert('Identificação gravada com sucesso');
            window.location='{$urlRedirect}';
        </script>
    ";
}


function alterarCpfNovo($dados) {
    global $db;

    $cpf = str_replace( array(".","-"), array("",""), $dados['novo_cpf'] );
    
    if($dados['novo_cpf']){
        $sql = "SELECT usucpf FROM seguranca.usuario WHERE usucpf='".$cpf."' AND usustatus = 'A'";
        $existe_us = $db->pegaUm($sql);
    }else{
        echo "CPF informado não esta cadastrado na base de dados do SIMEC.";
        exit();
    }
    
    if($existe_us){
        $sql = "Select * From projovemcampo.identificacaosecretario Where secocpf='".$existe_us."' and apcid='".$_SESSION['projovemcampo']['apcid']."'";
        $existe_pro = $db->executar($sql);
    }
    if($existe_pro['secacpf'] && $existe_pro['apcid']){
        $sql .= "UPDATE projovemcampo.identificacaosecretario SET secocpf='".$existe_us."' WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'";
        $db->executar($sql);
        $db->commit();
        echo "Usuário atualizado com sucesso.";
        exit();
    }else{  
        $sql = "UPDATE projovemcampo.identificacaosecretario SET secocpf='".$cpf."' WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'";
        $result = $db->executar($sql);
        $db->commit();
        echo "Dados Gravados com sucesso!";
        exit();
    }
}

function atualizarIdentificacao($dados) {
	global $db;
	
	$sql = "UPDATE projovemcampo.identificacaosecretario
                    SET isecep='".str_replace(array("-"),array(""),$dados['secacep'])."', 
                        iseendereco='".$dados['secaendereco']."', 
                        isenumero='".$dados['secanumero']."', 
                        isecomplemento=".(($dados['secacomplemento'])?"'".$dados['secacomplemento']."'":"NULL").", 
                        isebairro='".$dados['secabairro']."', 
                        iseuf='".$dados['secauf']."', 
                        isemunicipio='".$dados['secamuncod']."', 
                        isetelefone='".$dados['secatelefoneddd'].str_replace(array("-"),array(""),$dados['secatelefone'])."', 
                        isecelular='".$dados['secacelularddd'].str_replace(array("-"),array(""),$dados['secacelular'])."', 
                        iserg='".$dados['secaregistrogeral']."', 
                        iseorgexp='".$dados['secaorgaoexpedidor']."' 
                  WHERE secocpf='".$dados['secacpf']."';
        ";
	$db->executar($sql);
	$db->commit();
	
	if( $_SESSION['projovemcampo']['muncod'] ){
		$cmecodibge = $_SESSION['projovemcampo']['muncod'];
	}else{
		$cmecodibge = $db->pegaUm("select estcod from territorios.estado where estuf = '{$_SESSION['projovemcampo']['estuf']}'");
		
		$sql = "select * from projovemcampo.cargameta where cmecodibge = '{$cmecodibge}' and ppuid = {$_SESSION['projovemcampo']['ppuid']}";
		
		$rsCargaMeta = $db->pegaLinha($sql);
		
		$rsCargaMeta['juventude'] = $rsCargaMeta['juventude'] ? $rsCargaMeta['juventude'] : '0';
		$rsCargaMeta['prisional'] = $rsCargaMeta['prisional'] ? $rsCargaMeta['prisional'] : '0';
		$rsCargaMeta['geral'] = $rsCargaMeta['geral'] ? $rsCargaMeta['geral'] : '0';
		
		$sql = 'delete from projovemcampo.metasdoprograma where cmeid = ' . $rsCargaMeta['cmeid'] . ' and tpmid in (7,10,13);';
		
		$sql .= "INSERT INTO projovemcampo.metasdoprograma(tpmid, ppuid, suaid, cmeid, metvalor,apcid)
				VALUES (7, {$_SESSION['projovemcampo']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['juventude']}', {$_SESSION['projovemcampo']['apcid']});";
		$sql .= "INSERT INTO projovemcampo.metasdoprograma(tpmid, ppuid, suaid, cmeid, metvalor,apcid)
				VALUES (10, {$_SESSION['projovemcampo']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['prisional']}', {$_SESSION['projovemcampo']['apcid']});";
		$sql .= "INSERT INTO projovemcampo.metasdoprograma(tpmid, ppuid, suaid, cmeid, metvalor,apcid)
				VALUES (13, {$_SESSION['projovemcampo']['ppuid']}, null, {$rsCargaMeta['cmeid']}, '{$rsCargaMeta['geral']}', {$_SESSION['projovemcampo']['apcid']});";
// 		 ver($sql);
		if ($sql) {
			$db->executar($sql);
			$db->commit();
		}
	}
	
	
	echo "
            <script>
                alert('Identificação gravada com sucesso');
                window.location='projovemcampo.php?modulo=principal/identificacao&acao=A';
            </script>
        ";
}

function aceitarTermo($dados) {
    global $db;
    
    $perfis = pegaPerfilGeral();

    $sql .= "UPDATE projovemcampo.adesaoprojovemcampo SET  adesaotermodata=NOW(), apctermoaceito='t' WHERE apcid='" . $_SESSION['projovemcampo']['apcid'] . "'";
    $db->executar($sql);
    $db->commit();
    
    $url = "projovemcampo.php?modulo=principal/termoAdesao&acao=A";
    
    echo "
            <script>
            alert('Termo foi aceito com sucesso');
            window.location='".$url."';
            </script>
        ";
}

function naoAceitarTermo() {
	global $db;
	
    $sql .= "UPDATE projovemcampo.adesaoprojovemcampo SET  apctermoaceito='f' WHERE apcid='" . $_SESSION['projovemcampo']['apcid'] . "'";
    $db->executar($sql);
    $db->commit();
    
	echo "<script>
			alert('Termo não foi aceito com sucesso');
			window.location='projovemcampo.php?modulo=principal/".(($_SESSION['projovemcampo']['estuf'])?"listaEstados":"").(($_SESSION['projovemcampo']['muncod'])?"listaMunicipios":"")."&acao=A';
		  </script>";
}


function inserirSugestaoAmpliacao($dados) {
    global $db;
// 	ver($dados,d);
// 	die;
    $apcid              = $_SESSION['projovemcampo']['apcid'];
    $suaverdade         = $dados['suaverdade'] == "sim" ? "TRUE" : "FALSE";
    $ppuid              = $_SESSION['projovemcampo']['ppuid'];
    
    if($dados['suaverdade'] == "sim"){
        $suametasugerida = $dados['suametasugerida'];
    }elseif($dados['suaverdade'] == "nao"){
        $suametasugerida = '0';
    }else{
        $suametasugerida = '0';
    }

    $sql_sug = "
        INSERT INTO projovemcampo.sugestaoampliacao(
                    apcid, suaverdade, suametasugerida, suastatus, ppuid)
               VALUES( '" . $apcid . "', " . $suaverdade . ", " . $suametasugerida . ", 'A', " . $ppuid . " ) returning suaid;
    ";          
    $suaid = $db->pegaUm( $sql_sug );
    $db->commit();
    
    if ($suaverdade && $suametasugerida != '0' && $suaid > 0) {
        $sql = 'delete from projovemcampo.metasdoprograma where suaid = ' . $suaid . ' and tpmid in (8, 14);';

        if ($_REQUEST['metaDestinada_sugerida'] == 'J') {
            $sql .= "
                Insert into projovemcampo.metasdoprograma (tpmid, apcid, ppuid, suaid, cmeid, metvalor) values (8, $apcid, {$_SESSION['projovemcampo']['ppuid']}, {$suaid}, null, " .$suametasugerida. ");
                Insert into projovemcampo.metasdoprograma (tpmid, apcid, ppuid, suaid, cmeid, metvalor) values (14, $apcid, {$_SESSION['projovemcampo']['ppuid']}, {$suaid}, null, 0);                
                    
            ";
        } elseif ($_REQUEST['metaDestinada_sugerida'] == 'P') {
            $sql .= "
                Insert into projovemcampo.metasdoprograma (tpmid, apcid, ppuid, suaid, cmeid, metvalor) values (8, $apcid, {$_SESSION['projovemcampo']['ppuid']}, {$suaid}, null, 0);
                Insert into projovemcampo.metasdoprograma (tpmid, apcid, ppuid, suaid, cmeid, metvalor) values (14, $apcid, {$_SESSION['projovemcampo']['ppuid']}, {$suaid}, null, " .$suametasugerida. ");
            ";
        }
        $metasPrograma = $db->executar( $sql );
    }else{
        $msg = "Processo Concluído com sucesso";
    }
    
    if($metasPrograma){
        if ( $suaid > 0 ) {
            $msg = "Dados Gravado com sucesso";
            $db->commit();
        }else{
            $msg = "Ocorreu algum problema com a gravação dos dados, tente novamente mais tarde ou entre em contado com o administrador do sistema";
        }  
    }
    
    if ( $dados['suaverdade'] == "sim" ) {
        $end = "projovemcampo.php?modulo=principal/sugestaoAmpliacao&acao=A";
    } elseif ( $dados['suaverdade'] == "nao" ) {
        $end = "projovemcampo.php?modulo=principal/sugestaoAmpliacao&acao=A";
    }

    echo "
        <script>
            alert('{$msg}');
            window.location='{$end}';
		</script>
    ";
}

function atualizarSugestaoAmpliacao($dados) {
    global $db;
    
    $suaid           = $dados['suaid'];
    $suaverdade      = $dados['suaverdade'] == "sim" ? "TRUE" : "FALSE";
    $suametasugerida = $dados['suaverdade'] == "sim" ? "'".$dados['suametasugerida']."'" : "NULL";
    $suametaajustada = $dados['suametaajustada'];
    if($dados['suametaajustada']){
        $updtajus = ",suametaajustada = ".(($dados['suaverdade'] == "sim") ? "'".$dados['suametaajustada']."'" : "NULL");
    }
// 	ver($_REQUEST['metaDestinada_ajustada']);
// 	die;
    $sql = "
        UPDATE projovemcampo.sugestaoampliacao
                SET suaverdade = ".$suaverdade.", 
                    suametasugerida=".$suametasugerida." 
                    {$updtajus}
        WHERE apcid='".$_SESSION['projovemcampo']['apcid']."';
    ";
                    
    if ($suaverdade && $dados['suametaajustada'] != '' && $dados['suaid'] != '') {
        $sql .= 'delete from projovemcampo.metasdoprograma where suaid = ' . $suaid . ' and tpmid in (9, 15);';

        if ($_REQUEST['metaDestinada_ajustada'] == 'J') {
            $sql .= "
                Insert into projovemcampo.metasdoprograma (tpmid, ppuid, apcid, suaid, cmeid, metvalor) values (9, {$_SESSION['projovemcampo']['ppuid']}, {$_SESSION['projovemcampo']['apcid']}, {$suaid}, null, " .$suametaajustada. ");
                Insert into projovemcampo.metasdoprograma (tpmid, ppuid, apcid, suaid, cmeid, metvalor) values (15, {$_SESSION['projovemcampo']['ppuid']}, {$_SESSION['projovemcampo']['apcid']}, {$suaid}, null, 0);                
				Insert into projovemcampo.metasdoprograma (tpmid, ppuid, apcid, suaid, cmeid, metvalor) values (8, {$_SESSION['projovemcampo']['ppuid']}, {$_SESSION['projovemcampo']['apcid']}, {$suaid}, null, " .$suametasugerida. ");
                Insert into projovemcampo.metasdoprograma (tpmid, ppuid, apcid, suaid, cmeid, metvalor) values (14, {$_SESSION['projovemcampo']['ppuid']}, {$_SESSION['projovemcampo']['apcid']}, {$suaid}, null, 0);
            ";
        } elseif ($_REQUEST['metaDestinada_ajustada'] == 'P') {
            $sql .= "
                Insert into projovemcampo.metasdoprograma (tpmid, ppuid, apcid, suaid, cmeid, metvalor) values (9, {$_SESSION['projovemcampo']['ppuid']}, {$_SESSION['projovemcampo']['apcid']}, {$suaid}, null, 0);
                Insert into projovemcampo.metasdoprograma (tpmid, ppuid, apcid, suaid, cmeid, metvalor) values (15, {$_SESSION['projovemcampo']['ppuid']}, {$_SESSION['projovemcampo']['apcid']}, {$suaid}, null, " .$suametaajustada. ");
                Insert into projovemcampo.metasdoprograma (tpmid, ppuid, apcid, suaid, cmeid, metvalor) values (8, {$_SESSION['projovemcampo']['ppuid']}, {$_SESSION['projovemcampo']['apcid']}, {$suaid}, null, 0);
                Insert into projovemcampo.metasdoprograma (tpmid, ppuid, apcid, suaid, cmeid, metvalor) values (14, {$_SESSION['projovemcampo']['ppuid']}, {$_SESSION['projovemcampo']['apcid']}, {$suaid}, null, " .$suametasugerida. ");
            ";
        }
    }      

//    ver($sql,d);

    if( $db->executar($sql) ){
        $sql .= "UPDATE projovemcampo.adesaoprojovemcampo SET apctermoaceitoajustado = FALSE WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'";
        if( $db->executar($sql) ){
            $mensagem = "Dados Gravados com sucesso!";
        }else{
            $mensagem = "Ocorreu algum problema com a gravação dos dados, tente novamente mais tarde ou entre em contado com o administrador do sistema";
        }
    }else{
        $mensagem = "Ocorreu algum problema com a gravação dos dados, tente novamente mais tarde ou entre em contado com o administrador do sistema";
    }
    $db->commit();
    echo "
        <script>
            alert('{$mensagem}');
            window.location='projovemcampo.php?modulo=principal/termoAdesaoAjustado&acao=A';
        </script>
    ";
}

function mascaraglobal($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(substr($mask,$masklen,1) == "#") {
				$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
				$valuelen--;
			} else {
				if(trim(substr($value,$valuelen,1)) != "") {
					$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}

function termoProjovemCampoEstado($dados) {
	
    global $db;
	
    $rsSecretaria = recuperaSecretariaPorUfMuncod();
;
    $rsMetas = recuperaMetasPorUfMuncod($dados);

    $dadosT = $db->pegaLinha("SELECT * FROM territorios.estado e 
                              JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge = e.estcod::numeric 
    						  INNER JOIN projovemcampo.secretaria as sec on sec.secaid = c.secaid
    						  INNER JOIN projovemcampo.secretario as seco on seco.secoid = sec.secoid
    						  WHERE e.estuf = '" . $_SESSION['projovemcampo']['estuf'] . "'
                              		");

    ?>
    <table class="tabela" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td>
                  <h3 style="text-align:center">MINISTÉRIO DA EDUCAÇÃO</h3><BR />
                <h4 style="text-align:center">GABINETE DO MINISTRO</h4><BR />
                <h4 style="text-align:center">TERMO DE ADESÃO AO PROGRAMA NACIONAL DE INCLUSÃO DE JOVENS -  PROJOVEM URBANO E / OU PROJOVEM CAMPO</h4>
<BR />
                <p>
                    O Distrito Federal/Estado/Município de  <b><?= $dadosT['estdescricao'] ?></b>,doravante denominado Ente Federado, por meio da sua Secretaria de 
                    Educação, CNPJ: <b><?= mascaraglobal($rsSecretaria['entnumcpfcnpj'], "##.###.###/####-##") ?></b> representado por seu (sua) 
                    Secretário(a), <b><?= $dados['seconome'] ?></b>, CPF nº <b><?= mascaraglobal($dados['secocpf'], "###.###.###-##") ?></b>, 
                    RG nº <b><?= $dados['seconumrg'] ?></b>, expedido por  <b><?= $dados['secoorgaoexprg'] ?></b>, com atribuição legal para representar o governador ou o prefeito neste ato e devidamente estabelecido à   
                    <b> <?= $dados['secaendereco'] . ", nº " . $dados['secanumero'] . ", " . $dados['secabairro'] . ", " . $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $dados['secamuncod'] . "'") . ", " . $dados['secauf']. ", " ?></b><b>CEP <?= mascaraglobal($dados['secacep'], "#####-###")?> </b>, e o Ministério da Educação, representado pelo Ministro de Estado, resolvem firmar o presente Termo de Adesão ao Programa Nacional de Inclusão de Jovens – Projovem Urbano e/ou Projovem Campo – Saberes da Terra, edição 2014, em conformidade, no que couber, com a Lei n.º 8.666, de 21 de junho de 1993, e a legislação correlata, consideradas as seguintes condições:
                </p>
<br/>
               <h5><strong>Cláusula Primeira – Do Objeto</strong></h5><br>
                <p>
                  O presente termo tem por objeto a adesão do Ente Federado ao Programa Nacional de Inclusão de Jovens – Projovem Urbano e/ou Projovem Campo - Saberes da Terra, 
                   instituído nos termos da Lei nº 11.692 de 10 de junho de 2008,  regulamentado pelo Decreto nº 6.629 de 4 de novembro de 2008 e pelo Decreto nº 7.649 de 21 de dezembro de 2011.
                </p>
<br>

                <h5><strong>Cláusula Segunda – DAS OBRIGAÇÕES DOS ENTES FEDERADOS:</strong></h5>
<br><br>
                <p>1. Os Entes Federados se comprometem a cumprir as seguintes diretrizes abaixo:</p><br>

               	<p>I -executar o Programa, por meio da sua secretaria de Educação, que deverá coordenar o desenvolvimento das ações de implementação do Programa, garantindo a necessária articulação com a rede de ensino, conforme seus Projetos Pedagógicos Integrados, as orientações da Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão – SECADI/MEC e de acordo com as Resoluções CD/FNDE/MEC Nº 8/2014 e Nº 11/2014;</p>
                <p>II - executar os recursos orçamentários repassados pelo Governo Federal exclusivamente na implementação do Programa, gerindo-os com eficiência, eficácia e transparência, visando a efetividade das ações;</p>
                <p>III - estabelecer como foco a aprendizagem, realizando todos os esforços necessários para garantir a certificação em Ensino Fundamental – EJA e em qualificação profissional como formação inicial dos jovens matriculados no Programa;</p>
                <p>IV - responsabilizar-se pela divulgação do Programa em nível local, inclusive quanto aos processos de matrícula a serem realizados pelo Ente Federado, mobilizando a comunidade e suas lideranças, os jovens, pais e responsáveis, bem como os meios políticos e administrativos;</p>
                <p>V - empreender esforços para viabilizar a expedição dos documentos necessários para a matrícula dos jovens a serem atendidos pelo Programa;</p>
                <p>VI -matricular os estudantes por meio de Sistema de Matrícula, Acompanhamento de Frequência e Certificação do Projovem Urbano e Campo disponibilizado pela Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC, sendo esta a única forma de garantir a inclusão dos jovens no Programa, bem como ser responsável pela fidedignidade das informações lançadas no referido sistema;</p>
                <p>VII - garantir o acesso e as condições de permanência das pessoas público-alvo da educação especial ao Programa, por meio da oferta do atendimento educacional especializado e oferta de recursos e serviços de acessibilidade;</p>
                <p>VIII - desenvolver os Projetos Pedagógicos Integrados das duas modalidades do Programa em suas três dimensões, garantindo sua execução conforme legislação do Projovem Urbano e do Projovem Campo – Saberes da Terra e orientações da Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC;</p>
                <p>IX - acompanhar cada beneficiário individualmente, no caso do Projovem Urbano, mediante registro mensal de frequência e de entrega de trabalhos, e no caso do Projovem Campo – Saberes da Terra, mediante registro mensal de frequência por meio do Sistema de Matrícula, Acompanhamento da Frequência e Certificação do Projovem Urbano e Campo;</p> 
                <p>X - prevenir e combater a evasão pelo acompanhamento individual das razões para a não frequência do educando e implantar medidas para superá-las;</p>
                <p>XI - concordar integralmente com os  termos  das  Resoluções CD/FNDE/MEC Nº 8/2014 e Nº 11/2014 publicadas no Diário Oficial da União em 16 de abril de 2014, que estabelece os critérios e as normas de transferência automática de recursos financeiros do Projovem Urbano e do Projovem Campo – Saberes da Terra para a execução das ações do Programa;</p>
                <p>XII - autorizar o FNDE/MEC a estornar ou bloquear valores creditados indevidamente na conta corrente do Programa em favor do Ente Federado, mediante solicitação direta ao agente financeiro depositário dos recursos ou procedendo ao desconto nas parcelas subsequentes;</p>
                <p>XIII - restituir ao FNDE/MEC, no prazo de dez dias úteis a contar do recebimento da notificação e na forma prevista nas Resoluções CD/FNDE/MEC Nº 8/2014 e Nº 11/2014, os valores creditados indevidamente ou objeto de eventual irregularidade constatada, quando inexistir saldo suficiente na conta corrente e não houver repasses futuros a serem efetuados;</p>
                <p>XIV - aplica-se ao presente termo de adesão o previsto no art. 30, § 5º e no art. 36, § 4º do Decreto nº 6.629/2008.</p>
<br>
                <h5><strong>Cláusula Terceira – DAS OBRIGAÇÕES DO ESTADO/DISTRITO FEDERAL</strong></h5><br><br>
                <p>1. O Estado/Distrito Federal se obriga a:</p>
                                
                <p>1.1 Atingir a seguinte meta de atendimento de jovens para o Projovem Urbano e/ou Projovem Campo - Saberes da Terra, edição 2014:</p>
<br>
 
 				<table border=1 align=center width=30%>
                    <tr>
                        <td  colspan="5"align="center"><b>Meta 2014</b></td> 
                    </tr>
                    <tr>
                        <td align="center"><b>Meta Total</b></td>
                         <?php 
                         	if( $_SESSION['projovemcampo']['estuf'] ) {
			                    $sql = "SELECT coalesce( metvalor , 0 ) as total	                              
										FROM territorios.estado as e
										INNER JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge = e.estcod::numeric 
										INNER JOIN projovemcampo.meta m ON m.apcid = c.apcid AND tpmid = 1
			                              WHERE estuf='" . $_SESSION['projovemcampo']['estuf'] . "'";
		
			                    $rsValoresMeta = $db->pegaLinha( $sql );
			                    ?>
		                    	<td align="center"><strong><?php echo $rsValoresMeta['total'];?></strong></td>
		                    <?php } ?>
                    </tr>
                   
                </table>
               
                <br>
                <p>1.2 Cumprir as seguintes diretrizes:</p>
                 <br>
               	<p>I - priorizar o atendimento aos jovens residentes nos municípios integrantes do Plano Juventude Viva, das políticas de enfrentamento à violência e das regiões impactadas pelas grandes obras do Governo Federal, bem como aos jovens catadores de resíduos sólidos e egressos do Programa Brasil Alfabetizado;</p>
                <p>II - priorizar o atendimento às jovens mulheres, no caso da oferta em unidades do sistema prisional;</p>
                <p>III - garantir o funcionamento do comitê gestor do Projovem Urbano, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos de políticas de juventude, das políticas para mulheres, da promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, além da Agenda de Desenvolvimento Integrado de Alfabetização e Educação de Jovens e Adultos, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>IV - garantir o funcionamento do comitê gestor do Projovem Campo – Saberes da Terra, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos locais de políticas de juventude, dos movimentos sociais do campo e dos colegiados territoriais, bem como do órgão local de políticas para mulheres, de promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins e da Agenda de Desenvolvimento Integrado de Alfabetização e Educação de Jovens e Adultos e dos Comitês, Fóruns e/ou Articulações Estaduais de Educação do Campo, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>V - assegurar que 50% dos membros do comitê gestor local do Projovem Campo – Saberes da Terra seja de representantes das entidades que compõem os Comitês, Fóruns e/ou Articulações Estaduais de Educação do Campo;</p>
                <p>VI - garantir a oferta de Educação de Jovens e Adultos – EJA/Ensino Médio aos jovens atendidos pelo Programa nas escolas de sua rede, proporcionando a continuidade de seus estudos.</p>
                <br><br>
                
                <h5><strong>Cláusula Quarta – DAS OBRIGAÇÕES DO MUNICÍPIO</strong></h5><br><br>
                
                <p>1. O <strong> Município </strong>se compromete a:</p>
                <br />                
                <p>1.1 Atingir a seguinte meta de atendimento de jovens para o Projovem Urbano e/ou Projovem Campo - Saberes da Terra, edição 2014:</p>
<br>
 
                <table border=1 align=center width=30%>
                    <tr>
                        <td  colspan="5"align="center"><b>Meta 2014</b></td> 
                    </tr>
                    <tr>
                        <tr>
                        <td align="center"><b>Meta Total</b></td>
                        <td align="center">Público Juventude Viva (anexo II) Projovem Urbano</td>
                        <td align="center">Público Unidades Prisionais Projovem Urbano</td>
                        <td align="center">Público Geral do Projovem Urbano</td>
                        <td align="center">Público Projovem Campo Saberes da Terra</td>
                    </tr>
                </table>
                
                 <p>1.2 Cumprir as seguintes diretrizes:</p><br>
                 
               <p>I - priorizar o atendimento nas escolas localizadas nas regiões impactadas por grandes obras do Governo Federal, nas regiões com maiores índices de violência contra a juventude negra e nas áreas de abrangência das políticas de enfrentamento à violência, bem como atender aos jovens catadores de resíduos sólidos e egressos do Programa Brasil Alfabetizado.</p>
                <p>II - garantir o funcionamento do comitê gestor do Projovem Urbano, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos de políticas de juventude, das políticas para mulheres, da promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>III - garantir o funcionamento do comitê gestor do Projovem Campo – Saberes da Terra, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos locais de políticas de juventude, dos movimentos sociais do campo e dos colegiados territoriais, bem como do órgão local de políticas para mulheres, de promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>IV - articular-se com as redes estaduais de ensino visando garantir a continuidade de estudos para os jovens atendidos pelo Programa.</p>
              
                <br>
                <h5><strong>Cláusula Quinta – DA RECISÃO</strong></h5>
                <p>O presente instrumento poderá ser denunciado a qualquer tempo, no interesse das partes, ou rescindido pelo não cumprimento das cláusulas e/ou condições, observado o disposto nos artigos 77 a 80 da Lei nº 8.666, de 21 de junho de 1993, no que couber, independentemente de interpelação judicial ou extrajudicial ou daquelas dispostas nos artigos 86 a 88 do mesmo diploma legal.</p>
                <br><br>
                
                <h5><strong>Cláusula Sexta – DA PUBLICAÇÃO</strong></h5>
                <p>Caberá à Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC proceder à publicação do presente Termo de Adesão no Diário Oficial da União – DOU, 
                	conforme estabelecido no parágrafo único do art. 61 da Lei nº 8.666, de 21 de junho de 1993.</p>
                <br><br><br>
                
                <h5><strong>Cláusula Sétima– DO FORO</strong></h5>
                <p>O foro competente para dirimir qualquer questão relativa a instrumento é o da Justiça Federal, Foro da cidade de Brasília/DF, Seção Judiciária do Distrito Federal.</p>
                <br><br><br>
                
				<?php 
				$sql ="select apctermoaceito from projovemcampo.adesaoprojovemcampo where apcid = ". $_SESSION['projovemcampo']['apcid'];
				$termoaceito = $db->pegaUm( $sql );
				
				if( $termoaceito == 'f' || $termoaceito == 'FALSE' ){?>
                	<p align=center>___________________________________, <?= data_extenso(); ?></p>
				<?php }else{ 
					$sql ="select adesaotermodata from projovemcampo.adesaoprojovemcampo where apcid = ". $_SESSION['projovemcampo']['apcid'];
					$dataadesao = $db->pegaUm( $sql );
					?>
					<p align=center style="color:blue;"> <strong>Termo Aceito pela Secretaria em <?php echo data_extenso($dataadesao); ?>. </strong></p>
				<?php } ?>
                <br><br>
                <p align=center>___________________________________________________________________</p>
                <p align=center><b>Secretário(a) Municipal/Estadual/Distrital de Educação</b></p>
                <br /><br /><br /><br />
                <p align=center><b> JOSÉ HENRIQUE PAIM FERNANDES </b> </p>
               <p align=center>Ministro de Estado da Educação</p>

                
            </td>
        </tr>
</table>
        
<?
}

function data_extenso ($data = false)
{
	if ($data)
	{
		$mes = date('m', strtotime($data));
	}
	else
	{
		$mes = date('m');
		$data = date('Y-m-d');
	}
	$meses = array
	(
			'01' => 'Janeiro',
			'02' => 'Fevereiro',
			'03' => 'Março',
			'04' => 'Abril',
			'05' => 'Maio',
			'06' => 'Junho',
			'07' => 'Julho',
			'08' => 'Agosto',
			'09' => 'Setembro',
			'10' => 'Outubro',
			'11' => 'Novembro',
			'12' => 'Dezembro'
	);
	
	return date('d', strtotime($data)) . ' de ' . $meses[$mes] . ' de ' . date('Y', strtotime($data));
}

function termoProjovemCampoMunicipio($dados) {
	
    global $db;
    
    $rsSecretaria = recuperaSecretariaPorUfMuncod();

    $rsMetas = recuperaMetasPorUfMuncod($dados);

    $dadosT = $db->pegaLinha("SELECT * FROM territorios.municipio m 
                              JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge::character(7) = m.muncod and apcesfera in ('M') 
    						  INNER JOIN projovemcampo.secretaria as sec on sec.secaid = c.secaid
    						  INNER JOIN projovemcampo.secretario as seco on seco.secoid = sec.secoid
    						  WHERE m.muncod = '" . $_SESSION['projovemcampo']['muncod'] . "'
							");
	if( !$rsSecretaria['entnumcpfcnpj'] || $rsSecretaria['entnumcpfcnpj'] == '' ){
		$secacnpj = formatar_cnpj(recuperaCNPJPrefeitura() );
	}else{
		$secacnpj = formatar_cnpj($rsSecretaria['entnumcpfcnpj']);
	}
    ?>
    <table class="tabela" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td>
                <h3 style="text-align:center">MINISTÉRIO DA EDUCAÇÃO</h3><BR />
                <h4 style="text-align:center">GABINETE DO MINISTRO</h4><BR />
                <h4 style="text-align:center">TERMO DE ADESÃO AO PROGRAMA NACIONAL DE INCLUSÃO DE JOVENS -  PROJOVEM URBANO E / OU PROJOVEM CAMPO</h4>
				<br />
                 <p>
                    O Distrito Federal/Estado/Município de  <b><?= $dadosT['estdescricao'] ?></b>,doravante denominado Ente Federado, por meio da sua Secretaria de 
                    Educação, CNPJ: <b><?= mascaraglobal($rsSecretaria['entnumcpfcnpj'], "##.###.###/####-##") ?></b> representado por seu (sua) 
                    Secretário(a), <b><?= $dados['seconome'] ?></b>, CPF nº <b><?= mascaraglobal($dados['secocpf'], "###.###.###-##") ?></b>, 
                    RG nº <b><?= $dados['seconumrg'] ?></b>, expedido por  <b><?= $dados['secoorgaoexprg'] ?></b>, com atribuição legal para representar o governador ou o prefeito neste ato e devidamente estabelecido à   
                    <b> <?= $dados['secaendereco'] . ", nº " . $dados['secanumero'] . ", " . $dados['secabairro'] . ", " . $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $dados['secamuncod'] . "'") . ", " . $dados['secauf']. ", " ?></b><b>CEP <?= mascaraglobal($dados['secacep'], "#####-###")?> </b>, e o Ministério da Educação, representado pelo Ministro de Estado, resolvem firmar o presente Termo de Adesão ao Programa Nacional de Inclusão de Jovens – Projovem Urbano e/ou Projovem Campo – Saberes da Terra, edição 2014, em conformidade, no que couber, com a Lei n.º 8.666, de 21 de junho de 1993, e a legislação correlata, consideradas as seguintes condições:
                </p>
<br/>
               <h5><strong>Cláusula Primeira – Do Objeto</strong></h5><br>
                <p>
                   O presente termo tem por objeto a adesão do Ente Federado ao Programa Nacional de Inclusão de 
                   Jovens – Projovem Urbano e/ou Projovem Campo - Saberes da Terra, instituído nos termos da Lei nº 11.692 de 10 de junho de 2008, 
                   regulamentado pelo Decreto nº 6.629 de 4 de novembro de 2008 e pelo Decreto nº 7.649 de 21 de dezembro de 2011. 
                </p>
<br>

                <h5><strong>Cláusula Segunda – DAS OBRIGAÇÕES DOS ENTES FEDERADOS:</strong></h5>
<br><br>
                <p>1. Os Entes Federados se comprometem a cumprir as seguintes diretrizes abaixo:</p><br>

                <p>I -executar o Programa, por meio da sua secretaria de Educação, que deverá coordenar o desenvolvimento das ações de implementação do Programa, garantindo a necessária articulação com a rede de ensino, conforme seus Projetos Pedagógicos Integrados, as orientações da Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão – SECADI/MEC e de acordo com as Resoluções CD/FNDE/MEC Nº 8/2014 e Nº 11/2014;</p>
                <p>II - executar os recursos orçamentários repassados pelo Governo Federal exclusivamente na implementação do Programa, gerindo-os com eficiência, eficácia e transparência, visando a efetividade das ações;</p>
                <p>III - estabelecer como foco a aprendizagem, realizando todos os esforços necessários para garantir a certificação em Ensino Fundamental – EJA e em qualificação profissional como formação inicial dos jovens matriculados no Programa;</p>
                <p>IV - responsabilizar-se pela divulgação do Programa em nível local, inclusive quanto aos processos de matrícula a serem realizados pelo Ente Federado, mobilizando a comunidade e suas lideranças, os jovens, pais e responsáveis, bem como os meios políticos e administrativos;</p>
                <p>V - empreender esforços para viabilizar a expedição dos documentos necessários para a matrícula dos jovens a serem atendidos pelo Programa;</p>
                <p>VI -matricular os estudantes por meio de Sistema de Matrícula, Acompanhamento de Frequência e Certificação do Projovem Urbano e Campo disponibilizado pela Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC, sendo esta a única forma de garantir a inclusão dos jovens no Programa, bem como ser responsável pela fidedignidade das informações lançadas no referido sistema;</p>
                <p>VII - garantir o acesso e as condições de permanência das pessoas público-alvo da educação especial ao Programa, por meio da oferta do atendimento educacional especializado e oferta de recursos e serviços de acessibilidade;</p>
                <p>VIII - desenvolver os Projetos Pedagógicos Integrados das duas modalidades do Programa em suas três dimensões, garantindo sua execução conforme legislação do Projovem Urbano e do Projovem Campo – Saberes da Terra e orientações da Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC;</p>
                <p>IX - acompanhar cada beneficiário individualmente, no caso do Projovem Urbano, mediante registro mensal de frequência e de entrega de trabalhos, e no caso do Projovem Campo – Saberes da Terra, mediante registro mensal de frequência por meio do Sistema de Matrícula, Acompanhamento da Frequência e Certificação do Projovem Urbano e Campo;</p> 
                <p>X - prevenir e combater a evasão pelo acompanhamento individual das razões para a não frequência do educando e implantar medidas para superá-las;</p>
                <p>XI - concordar integralmente com os  termos  das  Resoluções CD/FNDE/MEC Nº 8/2014 e Nº 11/2014 publicadas no Diário Oficial da União em 16 de abril de 2014, que estabelece os critérios e as normas de transferência automática de recursos financeiros do Projovem Urbano e do Projovem Campo – Saberes da Terra para a execução das ações do Programa;</p>
                <p>XII - autorizar o FNDE/MEC a estornar ou bloquear valores creditados indevidamente na conta corrente do Programa em favor do Ente Federado, mediante solicitação direta ao agente financeiro depositário dos recursos ou procedendo ao desconto nas parcelas subsequentes;</p>
                <p>XIII - restituir ao FNDE/MEC, no prazo de dez dias úteis a contar do recebimento da notificação e na forma prevista nas Resoluções CD/FNDE/MEC Nº 8/2014 e Nº 11/2014, os valores creditados indevidamente ou objeto de eventual irregularidade constatada, quando inexistir saldo suficiente na conta corrente e não houver repasses futuros a serem efetuados;</p>
                <p>XIV - aplica-se ao presente termo de adesão o previsto no art. 30, § 5º e no art. 36, § 4º do Decreto nº 6.629/2008.</p>
<br>
                <h5><strong>Cláusula Terceira – DAS OBRIGAÇÕES DO ESTADO/DISTRITO FEDERAL</strong></h5><br><br>
                <p>1. O Estado/Distrito Federal se obriga a:</p>
                                
                <p>1.1 Atingir a seguinte meta de atendimento de jovens para o Projovem Urbano e/ou Projovem Campo - Saberes da Terra, edição 2014:</p>
<br>
 
                <table border=1 align=center width=30%>
                    <tr>
                        <td  colspan="5"align="center"><b>Meta 2014</b></td> 
                    </tr>
                    <tr>
                        <td align="center"><b>Meta Total</b></td>
                        <td align="center">--</strong></td>
                    </tr>
                   
                </table>
                 <br>
                <p>1.2 Cumprir as seguintes diretrizes:</p>
                 <br>
                <p>I - priorizar o atendimento aos jovens residentes nos municípios integrantes do Plano Juventude Viva, das políticas de enfrentamento à violência e das regiões impactadas pelas grandes obras do Governo Federal, bem como aos jovens catadores de resíduos sólidos e egressos do Programa Brasil Alfabetizado;</p>
                <p>II - priorizar o atendimento às jovens mulheres, no caso da oferta em unidades do sistema prisional;</p>
                <p>III - garantir o funcionamento do comitê gestor do Projovem Urbano, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos de políticas de juventude, das políticas para mulheres, da promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, além da Agenda de Desenvolvimento Integrado de Alfabetização e Educação de Jovens e Adultos, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>IV - garantir o funcionamento do comitê gestor do Projovem Campo – Saberes da Terra, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos locais de políticas de juventude, dos movimentos sociais do campo e dos colegiados territoriais, bem como do órgão local de políticas para mulheres, de promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins e da Agenda de Desenvolvimento Integrado de Alfabetização e Educação de Jovens e Adultos e dos Comitês, Fóruns e/ou Articulações Estaduais de Educação do Campo, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>V - assegurar que 50% dos membros do comitê gestor local do Projovem Campo – Saberes da Terra seja de representantes das entidades que compõem os Comitês, Fóruns e/ou Articulações Estaduais de Educação do Campo;</p>
                <p>VI - garantir a oferta de Educação de Jovens e Adultos – EJA/Ensino Médio aos jovens atendidos pelo Programa nas escolas de sua rede, proporcionando a continuidade de seus estudos.</p>
                
                <br><br>
                <h5><strong>Cláusula Quarta – DAS OBRIGAÇÕES DO MUNICÍPIO</strong></h5><br><br>
                
                <p>1. O <strong> Município </strong>se compromete a:</p>
                <br />                
                <p>1.1 Atingir a seguinte meta de atendimento de jovens para o Projovem Urbano e/ou Projovem Campo - Saberes da Terra, edição 2014:</p>
<br>
               <table border=1 align=center width=30%>
                    <tr>
                        <td  colspan="5"align="center"><b>Meta 2014</b></td> 
                    </tr>
                    <tr>
                        <td align="center"><b>Meta Total</b></td>
                        <?php 
                         	if( $_SESSION['projovemcampo']['muncod'] ) {
			                    $sql = "SELECT coalesce( metvalor , 0 ) as total	                              
										FROM territorios.municipio as mun
										INNER JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge = mun.muncod::numeric(7) 
										INNER JOIN projovemcampo.meta m ON m.apcid = c.apcid AND tpmid = 1
			                              WHERE muncod='" . $_SESSION['projovemcampo']['muncod'] . "'";
		
			                    $rsValoresMeta = $db->pegaLinha( $sql );
			                    ?>
		                    	<td align="center"><strong><?php echo $rsValoresMeta['total'];?></strong></td>
		                  <?php } ?>
                    </tr>
                     
                </table>
                   <p>1.2 Cumprir as seguintes diretrizes:</p><br>
                 
                <p>I - priorizar o atendimento nas escolas localizadas nas regiões impactadas por grandes obras do Governo Federal, nas regiões com maiores índices de violência contra a juventude negra e nas áreas de abrangência das políticas de enfrentamento à violência, bem como atender aos jovens catadores de resíduos sólidos e egressos do Programa Brasil Alfabetizado.</p>
                <p>II - garantir o funcionamento do comitê gestor do Projovem Urbano, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos de políticas de juventude, das políticas para mulheres, da promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>III - garantir o funcionamento do comitê gestor do Projovem Campo – Saberes da Terra, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos locais de políticas de juventude, dos movimentos sociais do campo e dos colegiados territoriais, bem como do órgão local de políticas para mulheres, de promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>IV - articular-se com as redes estaduais de ensino visando garantir a continuidade de estudos para os jovens atendidos pelo Programa.</p>
              
                <br>
                <h5><strong>Cláusula Quinta – DA RECISÃO</strong></h5>
                <p>O presente instrumento poderá ser denunciado a qualquer tempo, no interesse das partes, ou rescindido pelo não cumprimento das cláusulas e/ou condições, observado o disposto nos artigos 77 a 80 da Lei nº 8.666, de 21 de junho de 1993, no que couber, independentemente de interpelação judicial ou extrajudicial ou daquelas dispostas nos artigos 86 a 88 do mesmo diploma legal.</p>
                <br><br>
                
                <h5><strong>Cláusula Sexta – DA PUBLICAÇÃO</strong></h5>
                <p>Caberá à Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC proceder à publicação do presente Termo de Adesão no Diário Oficial da União – DOU, 
                	conforme estabelecido no parágrafo único do art. 61 da Lei nº 8.666, de 21 de junho de 1993.</p>
                <br><br><br>
                
                <h5><strong>Cláusula Sétima– DO FORO</strong></h5>
                <p>O foro competente para dirimir qualquer questão relativa a instrumento é o da Justiça Federal, Foro da cidade de Brasília/DF, Seção Judiciária do Distrito Federal.</p>
                <br><br><br>
                
				<?php 
				$sql ="select apctermoaceito from projovemcampo.adesaoprojovemcampo where apcid = ". $_SESSION['projovemcampo']['apcid'];
				$termoaceito = $db->pegaUm( $sql );
				
				if( $termoaceito == 'f' || $termoaceito == 'FALSE' ){?>
                	<p align=center>___________________________________, <?= data_extenso(); ?></p>
				<?php }else{ 
					$sql ="select adesaotermodata from projovemcampo.adesaoprojovemcampo where apcid = ". $_SESSION['projovemcampo']['apcid'];
					$dataadesao = $db->pegaUm( $sql );
					?>
					<p align=center style="color:blue;"> <strong>Termo Aceito pela Secretaria em <?php echo data_extenso($dataadesao); ?>. </strong></p>
				<?php } ?>
                <br><br>
                <p align=center>___________________________________________________________________</p>
                <p align=center><b>Secretário(a) Municipal/Estadual/Distrital de Educação</b></p>
                <br /><br /><br /><br />
                <p align=center><b> JOSÉ HENRIQUE PAIM FERNANDES </b> </p>
               <p align=center>Ministro de Estado da Educação</p>
            </td>
        </tr>
</table>
        
<?
}


function termoAjustadoProjovemCampoEstado($dados) {

	global $db;

	$rsSecretaria = recuperaSecretariaPorUfMuncod();

	$rsMetas = recuperaMetasPorUfMuncod($dados);

	$dadosT = $db->pegaLinha("SELECT * FROM territorios.estado e
                              JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge = e.estcod::numeric
    						  INNER JOIN projovemcampo.secretaria as sec on sec.secaid = c.secaid
    						  INNER JOIN projovemcampo.secretario as seco on seco.secoid = sec.secoid
    						  WHERE e.estuf = '" . $_SESSION['projovemcampo']['estuf'] . "'
                              		");

	?>
    <table class="tabela" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td>
                   <h3 style="text-align:center">MINISTÉRIO DA EDUCAÇÃO</h3><BR />
                <h4 style="text-align:center">GABINETE DO MINISTRO</h4><BR />
                <h4 style="text-align:center">TERMO DE ADESÃO AO PROGRAMA NACIONAL DE INCLUSÃO DE JOVENS -  PROJOVEM URBANO E / OU PROJOVEM CAMPO</h4>
<BR />
                <p>
                    O Estado/Distrito Federal/Municipio do <b><?= $dadosT['estdescricao'] ?></b> ,doravante denominado Ente Federado, por meio da sua Secretaria de 
                    Educação, CNPJ: <b><?= mascaraglobal($rsSecretaria['entnumcpfcnpj'], "##.###.###/####-##") ?></b> representado por seu (sua) 
                    Secretário(a), <b><?= $dados['seconome'] ?></b>, CPF nº <b><?= mascaraglobal($dados['secocpf'], "###.###.###-##") ?></b>, 
                    RG nº <b><?= $dados['seconumrg'] ?></b>, expedido por  <b><?= $dados['secoorgaoexprg'] ?></b>, devidamente estabelecido à  
                    <b><?= $dados['secaendereco'] . ", nº " . $dados['secanumero'] . ", " . $dados['secabairro'] . ", " . $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $dados['secamuncod']  . "'")  . " " . $dados['secauf'] . " CEP: " . mascaraglobal($dados['secacep'], "#####-###");?></b>, e o Ministério da Educação, representado pelo Ministro de Estado, resolve firmar o presente Termo de Adesão ao Programa Nacional de Inclusão de Jovens – Projovem Urbano e/ou Projovem Campo – Saberes da Terra, edição 2014, em conformidade, no que couber, com a Lei n.º 8.666, de 21 de junho de 1993, e a legislação correlata, consideradas as seguintes condições:
                </p>
<br/>
                <h5><strong>Cláusula Primeira – Do Objeto</strong></h5><br>
                <p>
                   O presente termo tem por objeto a adesão do Ente Federado ao Programa Nacional de Inclusão de 
                   Jovens – Projovem Urbano e/ou Projovem Campo - Saberes da Terra, instituído nos termos da Lei nº 11.692 de 10 de junho de 2008, 
                   regulamentado pelo Decreto nº 6.629 de 4 de novembro de 2008 e pelo Decreto nº 7.649 de 21 de dezembro de 2011. 
                </p>
<br>

                <h5><strong>Cláusula Segunda – DAS OBRIGAÇÕES DOS ENTES FEDERADOS:</strong></h5>
<br><br>
                <p>1. Os Entes Federados se comprometem a cumprir as seguintes diretrizes abaixo:</p><br>

                <p>I -executar o Programa, por meio da sua Secretaria de Educação, que deverá coordenar o desenvolvimento das ações de implementação do Programa, garantindo a necessária articulação com a rede de ensino, conforme seus Projetos Pedagógicos Integrados, as orientações da Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão – SECADI/MEC e de acordo com Resolução CD/FNDE Nº  de 2013;</p>
                <p>II - executar os recursos orçamentários repassados pelo Governo Federal exclusivamente na implementação do Programa, gerindo-os com eficiência, eficácia e transparência, visando a efetividade das ações;</p>
                <p>III - estabelecer como foco a aprendizagem, realizando todos os esforços necessários para garantir a certificação em Ensino Fundamental – EJA e em qualificação profissional como formação inicial dos jovens matriculados no Programa;</p>
                <p>IV - responsabilizar-se pela divulgação do Programa em nível local, inclusive quanto aos processos de matrícula a serem realizados pelo Ente Federado, mobilizando a comunidade e suas lideranças, os jovens, pais e responsáveis, bem como os meios políticos e administrativos;</p>
                <p>V - empreender esforços para viabilizar a expedição dos documentos necessários para a matrícula dos jovens a serem atendidos pelo Programa;</p>
                <p>VI -matricular os estudantes por meio de Sistema de Matrícula, Acompanhamento de Frequência e Certificação do Projovem Urbano e Campo disponibilizado pela Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC, sendo esta a única forma de garantir a inclusão dos jovens no Programa, bem como ser responsável pela fidedignidade das informações lançadas no referido sistema;</p>
                <p>VII - garantir o acesso e as condições de permanência das pessoas público-alvo da educação especial ao Programa, por meio da oferta do atendimento educacional especializado e oferta de recursos e serviços de acessibilidade;</p>
                <p>VIII - desenvolver os Projetos Pedagógicos Integrados das duas modalidades do Programa em suas três dimensões, garantindo sua execução conforme legislação do Projovem Urbano e do Projovem Campo – Saberes da Terra e orientações da Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC;</p>
                <p>IX - acompanhar cada beneficiário individualmente, no caso do Projovem Urbano, mediante registro mensal de frequência e de entrega de trabalhos, e no caso do Projovem Campo – Saberes da Terra, mediante registro mensal de frequência por meio do Sistema de Matrícula, Acompanhamento da Frequência e Certificação do Projovem Urbano e Campo;</p> 
                <p>X - prevenir e combater a evasão pelo acompanhamento individual das razões para a não frequência do educando e implantar medidas para superá-las;</p>
                <p>XI - concordar integralmente com os termos da Resolução CD/FNDE Nº de 2013 publicada no Diário Oficial da União em, que estabelece os critérios e as normas de transferência automática de recursos financeiros do Projovem Urbano e do Projovem Campo – Saberes da Terra para a execução das ações do Programa; </p>
                <p>XII - autorizar o FNDE/MEC a estornar ou bloquear valores creditados indevidamente na conta corrente do Programa em favor do Ente Federado, mediante solicitação direta ao agente financeiro depositário dos recursos ou procedendo ao desconto nas parcelas subsequentes;</p>
                <p>XIII - restituir ao FNDE/MEC, no prazo de dez dias úteis a contar do recebimento da notificação e na forma prevista nos §§ 17 a 20 do art. 18 da referida Resolução, os valores creditados indevidamente ou objeto de eventual irregularidade constatada, quando inexistir saldo suficiente na conta corrente e não houver repasses futuros a serem efetuados;</p>
                <p>XIV - Aplica-se ao presente termo de adesão o previsto no art. 30, § 5º e no art. 36, § 4º do Decreto n.º 6.629/2008.</p>
<br>
                <h5><strong>Cláusula Terceira – DAS OBRIGAÇÕES DO ESTADO/DISTRITO FEDERAL</strong></h5><br><br>
                <p>1. O Estado/Distrito Federal se obriga a:</p>
                                
                <p>1.1 Atingir a seguinte meta de atendimento de jovens para o Projovem Urbano e/ou Projovem Campo - Saberes da Terra, edição 2014:</p>
<br>
 
 				<table border=1 align=center width=30%>
                    <tr>
                        <td  colspan="5"align="center"><b>Meta 2014</b></td> 
                    </tr>
                    <tr>
                        <td align="center"><b>Meta Total</b></td>
                         <?php 
                         	if( $_SESSION['projovemcampo']['estuf'] ) {
			                    $sql = "SELECT coalesce( metvalor , 0 ) as total	                              
										FROM territorios.estado as e
										INNER JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge = e.estcod::numeric 
										INNER JOIN projovemcampo.meta m ON m.apcid = c.apcid
			                              WHERE estuf='" . $_SESSION['projovemcampo']['estuf'] . "' and m.tpmid = 3 ";
		
			                    $rsValoresMeta = $db->pegaLinha( $sql );
			                    ?>
		                    	<td align="center"><strong><?php echo $rsValoresMeta['total'];?></strong></td>
		                    <?php } ?>
                    </tr>
                   
                </table>
               <br>
                <p>1.2 Cumprir as seguintes diretrizes:</p>
                 <br>
                <p>I - priorizar o atendimento aos jovens residentes nos municípios integrantes do Plano Juventude Viva, das políticas de enfrentamento à violência e das regiões impactadas pelas grandes obras do Governo Federal, bem como aos jovens catadores de resíduos sólidos e egressos do Programa Brasil Alfabetizado; </p>
                <p>II - priorizar o atendimento às jovens mulheres, no caso da oferta em unidades do sistema prisional;</p>
                <p>III - garantir o funcionamento do comitê gestor do Projovem Urbano, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos de políticas de juventude, das políticas para mulheres, da promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, além da Agenda de Desenvolvimento Integrado de Alfabetização e Educação de Jovens e Adultos, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>IV - garantir o funcionamento do comitê gestor do Projovem Campo – Saberes da Terra, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos locais de políticas de juventude, dos movimentos sociais do campo e dos colegiados territoriais, bem como do órgão local de políticas para mulheres, de promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins e da Agenda de Desenvolvimento Integrado de Alfabetização e Educação de Jovens e Adultos e dos Comitês, Fóruns e/ou Articulações Estaduais de Educação do Campo, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>V - assegurar que 50% dos membros do comitê gestor local do Projovem Campo – Saberes da Terra seja de representantes das entidades que compõem os Comitês, Fóruns e/ou Articulações Estaduais de Educação do Campo;</p>
                <p>VI - garantir a oferta de Educação de Jovens e Adultos – EJA/Ensino Médio aos jovens atendidos pelo Programa nas escolas de sua rede, proporcionando a continuidade de seus estudos.</p>
                
                <br><br>
                <h5><strong>Cláusula Quarta – DAS OBRIGAÇÕES DO MUNICÍPIO</strong></h5><br><br>
                
                <p>1. O <strong> Município </strong>se compromete a:</p>
                <br />                
                <p>1.1 Atingir a seguinte meta de atendimento de jovens para o Projovem Urbano e/ou Projovem Campo - Saberes da Terra, edição 2014:</p>
<br>
                 <table border=1 align=center width=30%>
                    <tr>
                        <td  colspan="5"align="center"><b>Meta 2014</b></td> 
                    </tr>
                    <tr>
                        <td align="center"><b>Meta Total</b></td>
                        <td align="center">--</strong></td>
                    </tr>
                   
                </table>
                
                 <p>1.2 Cumprir as seguintes diretrizes:</p><br>
                 
                <p>I - priorizar o atendimento nas escolas localizadas nas regiões impactadas por grandes obras do Governo Federal, nas regiões com maiores índices de violência contra a juventude negra e nas áreas de abrangência das políticas de enfrentamento à violência, bem como atender aos jovens catadores de resíduos sólidos e egressos do Programa Brasil Alfabetizado.  </p>
                <p>II - garantir o funcionamento do comitê gestor do Projovem Urbano, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos de políticas de juventude, das políticas para mulheres, da promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>III - garantir o funcionamento do comitê gestor do Projovem Campo – Saberes da Terra, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos locais de políticas de juventude, dos movimentos sociais do campo e dos colegiados territoriais, bem como do órgão local de políticas para mulheres, de promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>IV - articular-se com as redes estaduais de ensino visando garantir a continuidade de estudos para os jovens atendidos pelo Programa.</p>
              
                <br>
                <h5><strong>Cláusula Quinta – DA RECISÃO</strong></h5>
                <p>O presente instrumento poderá ser denunciado a qualquer tempo, no interesse das partes, ou rescindido pelo não cumprimento das cláusulas e/ou condições, observado o disposto nos artigos 77 a 80 da Lei nº 8.666, de 21 de junho de 1993, e o Decreto nº 6.170, 25 de julho de 2007, no que couber, independentemente de interpelação judicial ou extrajudicial ou daquelas dispostas nos artigos 86 a 88 do mesmo diploma legal.</p>
                <br><br>
                
                <h5><strong>Cláusula Sexta – DA PUBLICAÇÃO</strong></h5>
                <p>Caberá à Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC proceder à publicação do presente Termo de Adesão no Diário Oficial da União – DOU, conforme estabelecido no parágrafo único do art. 61 da Lei nº 8.666, de 21 de junho de 1993.</p>
                <br><br><br>
                
                <h5><strong>Cláusula Sétima– DO FORO</strong></h5>
                <p>O foro competente para dirimir qualquer questão relativa a instrumento é o da Justiça Federal, Foro da cidade de Brasília/DF, Seção Judiciária do Distrito Federal.</p>
                <br><br><br>
                
				<?php 
				$sql ="select apctermoajustadoaceito from projovemcampo.adesaoprojovemcampo where apcid = ". $_SESSION['projovemcampo']['apcid'];
				$termoaceito = $db->pegaUm( $sql );
				
				if( $termoaceito == 'f' || $termoaceito == '' ){?>
                	<p align=center>___________________________________, <?= data_extenso(); ?></p>
				<?php }else{

					$sql ="select adesaotermoajustadodata from projovemcampo.adesaoprojovemcampo where apcid = ". $_SESSION['projovemcampo']['apcid'];
					$dataadesao = $db->pegaUm( $sql );?>
					<p align=center style="color:blue;"> <strong>Termo ajustado Aceito pela Secretaria em <?php echo data_extenso($dataadesao); ?>. </strong></p>
				
				<?php } ?>
                <br><br>
                <p align=center>___________________________________________________________________</p>
                <p align=center><b>Secretário(a) Municipal/Estadual/Distrital de Educação</b></p>
                <br /><br /><br /><br />
                <p align=center><b> JOSÉ HENRIQUE PAIM FERNANDES </b> </p>
               <p align=center>Ministro de Estado da Educação</p>
                
            </td>
        </tr>
</table>
       <?
}


function termoAjustadoProjovemCampoMunicipio($dados) {
	
    global $db;
    
    $rsSecretaria = recuperaSecretariaPorUfMuncod();

    $rsMetas = recuperaMetasPorUfMuncod($dados);

    $dadosT = $db->pegaLinha("SELECT * FROM territorios.municipio m 
                              JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge::character(7) = m.muncod and apcesfera in ('M') 
    						  INNER JOIN projovemcampo.secretaria as sec on sec.secaid = c.secaid
    						  INNER JOIN projovemcampo.secretario as seco on seco.secoid = sec.secoid
    						  WHERE m.muncod = '" . $_SESSION['projovemcampo']['muncod'] . "'
							");
	if( !$rsSecretaria['entnumcpfcnpj'] || $rsSecretaria['entnumcpfcnpj'] == '' ){
		$secacnpj = formatar_cnpj(recuperaCNPJPrefeitura() );
	}else{
		$secacnpj = formatar_cnpj($rsSecretaria['entnumcpfcnpj']);
	}
    ?>
    <table class="tabela" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td>
                <h3 style="text-align:center">MINISTÉRIO DA EDUCAÇÃO</h3><BR />
                <h4 style="text-align:center">GABINETE DO MINISTRO</h4><BR />
                <h4 style="text-align:center">TERMO DE ADESÃO AO PROGRAMA NACIONAL DE INCLUSÃO DE JOVENS -  PROJOVEM URBANO E / OU PROJOVEM CAMPO</h4>
				<br />
                <p>
                    O Estado/Distrito Federal/Municipio do <b><?= $dadosT['mundescricao'] ?> </b>,doravante denominado Ente Federado, por meio da sua Secretaria de 
                    Educação, CNPJ: <b><?= $secacnpj;?></b> representado por seu (sua) 
                    Secretário(a), <b><?= $dados['seconome'] ?></b>, CPF nº <b><?= mascaraglobal($dados['secocpf'], "###.###.###-##") ?></b>, 
                    RG nº <b><?= $dados['seconumrg'] ?></b>, expedido por  <b><?= $dados['secoorgaoexprg'] ?></b>, devidamente estabelecido à  
                    <b><?=$dados['secaendereco'] . ", nº " . $dados['secanumero'] . ", " . $dados['secabairro'] . ", " . $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $dados['secamuncod'] . "'") . ", " . $dados['secauf'] . " CEP: " . mascaraglobal($dados['secacep'], "#####-###"); ?></b>, e o Ministério da Educação, representado pelo Ministro de Estado, resolve firmar o presente Termo de Adesão ao Programa Nacional de Inclusão de Jovens – Projovem Urbano e/ou Projovem Campo – Saberes da Terra, edição 2014, em conformidade, no que couber, com a Lei n.º 8.666, de 21 de junho de 1993, e a legislação correlata, consideradas as seguintes condições:
                </p>
<br/>
                <h5><strong>Cláusula Primeira – Do Objeto</strong></h5><br>
                <p>
                   O presente termo tem por objeto a adesão do Ente Federado ao Programa Nacional de Inclusão de 
                   Jovens – Projovem Urbano e/ou Projovem Campo - Saberes da Terra, instituído nos termos da Lei nº 11.692 de 10 de junho de 2008, 
                   regulamentado pelo Decreto nº 6.629 de 4 de novembro de 2008 e pelo Decreto nº 7.649 de 21 de dezembro de 2011. 
                </p>
<br>

                <h5><strong>Cláusula Segunda – DAS OBRIGAÇÕES DOS ENTES FEDERADOS:</strong></h5>
<br><br>
                <p>1. Os Entes Federados se comprometem a cumprir as seguintes diretrizes abaixo:</p><br>

                <p>I -executar o Programa, por meio da sua Secretaria de Educação, que deverá coordenar o desenvolvimento das ações de implementação do Programa, garantindo a necessária articulação com a rede de ensino, conforme seus Projetos Pedagógicos Integrados, as orientações da Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão – SECADI/MEC e de acordo com Resolução CD/FNDE Nº  de 2013;</p>
                <p>II - executar os recursos orçamentários repassados pelo Governo Federal exclusivamente na implementação do Programa, gerindo-os com eficiência, eficácia e transparência, visando a efetividade das ações;</p>
                <p>III - estabelecer como foco a aprendizagem, realizando todos os esforços necessários para garantir a certificação em Ensino Fundamental – EJA e em qualificação profissional como formação inicial dos jovens matriculados no Programa;</p>
                <p>IV - responsabilizar-se pela divulgação do Programa em nível local, inclusive quanto aos processos de matrícula a serem realizados pelo Ente Federado, mobilizando a comunidade e suas lideranças, os jovens, pais e responsáveis, bem como os meios políticos e administrativos;</p>
                <p>V - empreender esforços para viabilizar a expedição dos documentos necessários para a matrícula dos jovens a serem atendidos pelo Programa;</p>
                <p>VI -matricular os estudantes por meio de Sistema de Matrícula, Acompanhamento de Frequência e Certificação do Projovem Urbano e Campo disponibilizado pela Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC, sendo esta a única forma de garantir a inclusão dos jovens no Programa, bem como ser responsável pela fidedignidade das informações lançadas no referido sistema;</p>
                <p>VII - garantir o acesso e as condições de permanência das pessoas público-alvo da educação especial ao Programa, por meio da oferta do atendimento educacional especializado e oferta de recursos e serviços de acessibilidade;</p>
                <p>VIII - desenvolver os Projetos Pedagógicos Integrados das duas modalidades do Programa em suas três dimensões, garantindo sua execução conforme legislação do Projovem Urbano e do Projovem Campo – Saberes da Terra e orientações da Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC;</p>
                <p>IX - acompanhar cada beneficiário individualmente, no caso do Projovem Urbano, mediante registro mensal de frequência e de entrega de trabalhos, e no caso do Projovem Campo – Saberes da Terra, mediante registro mensal de frequência por meio do Sistema de Matrícula, Acompanhamento da Frequência e Certificação do Projovem Urbano e Campo;</p> 
                <p>X - prevenir e combater a evasão pelo acompanhamento individual das razões para a não frequência do educando e implantar medidas para superá-las;</p>
                <p>XI - concordar integralmente com os termos da Resolução CD/FNDE Nº de 2013 publicada no Diário Oficial da União em, que estabelece os critérios e as normas de transferência automática de recursos financeiros do Projovem Urbano e do Projovem Campo – Saberes da Terra para a execução das ações do Programa; </p>
                <p>XII - autorizar o FNDE/MEC a estornar ou bloquear valores creditados indevidamente na conta corrente do Programa em favor do Ente Federado, mediante solicitação direta ao agente financeiro depositário dos recursos ou procedendo ao desconto nas parcelas subsequentes;</p>
                <p>XIII - restituir ao FNDE/MEC, no prazo de dez dias úteis a contar do recebimento da notificação e na forma prevista nos §§ 17 a 20 do art. 18 da referida Resolução, os valores creditados indevidamente ou objeto de eventual irregularidade constatada, quando inexistir saldo suficiente na conta corrente e não houver repasses futuros a serem efetuados;</p>
                <p>XIV - Aplica-se ao presente termo de adesão o previsto no art. 30, § 5º e no art. 36, § 4º do Decreto n.º 6.629/2008.</p>
<br>
                <h5><strong>Cláusula Terceira – DAS OBRIGAÇÕES DO ESTADO/DISTRITO FEDERAL</strong></h5><br><br>
                <p>1. O Estado/Distrito Federal se obriga a:</p>
                                
                <p>1.1 Atingir a seguinte meta de atendimento de jovens para o Projovem Urbano e/ou Projovem Campo - Saberes da Terra, edição 2014:</p>
<br>
 
                <table border=1 align=center width=30%>
                    <tr>
                        <td  colspan="5"align="center"><b>Meta 2014</b></td> 
                    </tr>
                    <tr>
                        <td align="center"><b>Meta Total</b></td>
                        <td align="center">--</strong></td>
                    </tr>
                   
                </table>
                 <br>
                <p>1.2 Cumprir as seguintes diretrizes:</p>
                 <br>
                <p>I - priorizar o atendimento aos jovens residentes nos municípios integrantes do Plano Juventude Viva, das políticas de enfrentamento à violência e das regiões impactadas pelas grandes obras do Governo Federal, bem como aos jovens catadores de resíduos sólidos e egressos do Programa Brasil Alfabetizado; </p>
                <p>II - priorizar o atendimento às jovens mulheres, no caso da oferta em unidades do sistema prisional;</p>
                <p>III - garantir o funcionamento do comitê gestor do Projovem Urbano, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos de políticas de juventude, das políticas para mulheres, da promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, além da Agenda de Desenvolvimento Integrado de Alfabetização e Educação de Jovens e Adultos, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>IV - garantir o funcionamento do comitê gestor do Projovem Campo – Saberes da Terra, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos locais de políticas de juventude, dos movimentos sociais do campo e dos colegiados territoriais, bem como do órgão local de políticas para mulheres, de promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins e da Agenda de Desenvolvimento Integrado de Alfabetização e Educação de Jovens e Adultos e dos Comitês, Fóruns e/ou Articulações Estaduais de Educação do Campo, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>V - assegurar que 50% dos membros do comitê gestor local do Projovem Campo – Saberes da Terra seja de representantes das entidades que compõem os Comitês, Fóruns e/ou Articulações Estaduais de Educação do Campo;</p>
                <p>VI - garantir a oferta de Educação de Jovens e Adultos – EJA/Ensino Médio aos jovens atendidos pelo Programa nas escolas de sua rede, proporcionando a continuidade de seus estudos.</p>
                
                <br><br>
                <h5><strong>Cláusula Quarta – DAS OBRIGAÇÕES DO MUNICÍPIO</strong></h5><br><br>
                
                <p>1. O <strong> Município </strong>se compromete a:</p>
                <br />                
                <p>1.1 Atingir a seguinte meta de atendimento de jovens para o Projovem Urbano e/ou Projovem Campo - Saberes da Terra, edição 2014:</p>
<br>
               <table border=1 align=center width=30%>
                    <tr>
                        <td  colspan="5"align="center"><b>Meta 2014</b></td> 
                    </tr>
                    <tr>
                        <td align="center"><b>Meta Total</b></td>
                        <?php 
                         	if( $_SESSION['projovemcampo']['muncod'] ) {
			                    $sql = "SELECT coalesce( metvalor , 0 ) as total	                              
										FROM territorios.municipio as mun
										INNER JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge = mun.muncod::numeric(7) 
										INNER JOIN projovemcampo.meta m ON m.apcid = c.apcid
			                              WHERE muncod='" . $_SESSION['projovemcampo']['muncod'] . "' and m.tpmid = 3";
		
			                    $rsValoresMeta = $db->pegaLinha( $sql );
			                    ?>
		                    	<td align="center"><strong><?php echo $rsValoresMeta['total'];?></strong></td>
		                  <?php } ?>
                    </tr>
                     
                </table>
                  <p>1.2 Cumprir as seguintes diretrizes:</p><br>
                 
                <p>I - priorizar o atendimento nas escolas localizadas nas regiões impactadas por grandes obras do Governo Federal, nas regiões com maiores índices de violência contra a juventude negra e nas áreas de abrangência das políticas de enfrentamento à violência, bem como atender aos jovens catadores de resíduos sólidos e egressos do Programa Brasil Alfabetizado.  </p>
                <p>II - garantir o funcionamento do comitê gestor do Projovem Urbano, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos de políticas de juventude, das políticas para mulheres, da promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>III - garantir o funcionamento do comitê gestor do Projovem Campo – Saberes da Terra, no âmbito local, sob coordenação da Secretaria de Educação, composto por representação do Conselho de Juventude, quando existir na localidade, dos órgãos locais de políticas de juventude, dos movimentos sociais do campo e dos colegiados territoriais, bem como do órgão local de políticas para mulheres, de promoção da igualdade racial, dos jovens participantes no Programa, das demais secretarias afins, para garantir efetividade ao acompanhamento e apoio à execução das ações do Programa, observada a intersetorialidade necessária para a execução dessas ações;</p>
                <p>IV - articular-se com as redes estaduais de ensino visando garantir a continuidade de estudos para os jovens atendidos pelo Programa.</p>
              
                <br>
                <h5><strong>Cláusula Quinta – DA RECISÃO</strong></h5>
                <p>O presente instrumento poderá ser denunciado a qualquer tempo, no interesse das partes, ou rescindido pelo não cumprimento das cláusulas e/ou condições, observado o disposto nos artigos 77 a 80 da Lei nº 8.666, de 21 de junho de 1993, e o Decreto nº 6.170, 25 de julho de 2007, no que couber, independentemente de interpelação judicial ou extrajudicial ou daquelas dispostas nos artigos 86 a 88 do mesmo diploma legal.</p>
                <br><br>
                
                <h5><strong>Cláusula Sexta – DA PUBLICAÇÃO</strong></h5>
                <p>Caberá à Secretaria de Educação Continuada, Alfabetização, Diversidade e Inclusão - SECADI/MEC proceder à publicação do presente Termo de Adesão no Diário Oficial da União – DOU, conforme estabelecido no parágrafo único do art. 61 da Lei nº 8.666, de 21 de junho de 1993.</p>
                <br><br><br>
                
                <h5><strong>Cláusula Sétima– DO FORO</strong></h5>
                <p>O foro competente para dirimir qualquer questão relativa a instrumento é o da Justiça Federal, Foro da cidade de Brasília/DF, Seção Judiciária do Distrito Federal.</p>
                <br><br><br>
                
				<?php 
				$sql ="select apctermoajustadoaceito from projovemcampo.adesaoprojovemcampo where apcid = ". $_SESSION['projovemcampo']['apcid'];
				$termoaceito = $db->pegaUm( $sql );
				
				if( $termoaceito == 'f' || $termoaceito == '' ){?>
                	<p align=center>___________________________________, <?= data_extenso(); ?></p>
				<?php }else{

					$sql ="select adesaotermoajustadodata from projovemcampo.adesaoprojovemcampo where apcid = ". $_SESSION['projovemcampo']['apcid'];
					$dataadesao = $db->pegaUm( $sql );?>
					<p align=center style="color:blue;"> <strong>Termo ajustado Aceito pela Secretaria em <?php echo data_extenso($dataadesao); ?>. </strong></p>
				
				<?php } ?>
                <br><br>
                <p align=center>___________________________________________________________________</p>
                <p align=center><b>Secretário(a) Municipal/Estadual/Distrital de Educação</b></p>
                <br /><br /><br /><br />
                <p align=center><b> JOSÉ HENRIQUE PAIM FERNANDES </b> </p>
               <p align=center>Ministro de Estado da Educação</p>
                
                
            </td>
        </tr>
</table>
        
<?
}

function inserirCursosQualificacaoEstado($dados) {
	global $db;
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script>
	function salvarCursosEstado() {
		if(document.getElementById('muncod').value=='') {
			alert('Selecione um municipio');
			return false;
		}
		
		selectAllOptions( document.getElementById( 'cofid' ) );
		
		document.getElementById( 'form' ).submit();
	}	
	</script>
	<form id="form" name="form" method="POST">
	<input type="hidden" name="requisicao" value="inserirMunicipioCursosEstado">
	<table class="tabela" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita">Município</td>
			<td><? $db->monta_combo('muncod', "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_SESSION['projovemcampo']['estuf']."'", 'S', 'Selecione', '', '', '', '200', 'S', 'muncod', '', $arcids[0]); ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Cursos</td>
			<td><? 
			$sql = "SELECT cofid as codigo, cofdesc as descricao FROM projovemcampo.cursoofertado WHERE cofstatus='A'";
			combo_popup( "cofid", $sql, "Cursos", "192x400", 0, array(), "", "S", false, false, 5, 400 );
			?></td>
		</tr>
		<tr>
			<td colspan="2" class="SubtituloCentro"  ><input type="button" name="salvar" value="Salvar" onclick="salvarCursosEstado();" /> </td>
		</tr>
	</table>
	</form>
	<?

}

function inserirMunicipioCursosEstado($dados) {
	global $db;
	if($dados['cofid']) {
		foreach($dados['cofid'] as $cof) {
			$sql = "INSERT INTO projovemcampo.cursoqualificacao(
				            qprid, cofid, muncod, cuqstatus)
				    VALUES ('".$_SESSION['projovemcampo']['qprid']."', 
				    		'".$cof."', 
				    		'".$dados['muncod']."', 'A');";
			
			$db->executar($sql);
		}
		
		$db->commit();
	}
	
	echo "<script>
			alert('Gravado com sucesso');
			window.opener.carregarListaCursosEstado();
			window.close();
		  </script>";
}



function carregarListaCursosEstado($dados) {
	global $db;
	
	$sql = "SELECT '<center><img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluirPoloMunicipio(\'projovemcampo.php?modulo=principal/planoImplementacao&acao=A&requisicao=excluirCursoMunicipioEstado&cuqid='||cuqid||'\');\"></center>' as acao, mun.mundescricao, cof.cofdesc FROM projovemcampo.cursoqualificacao cq 
			INNER JOIN territorios.municipio mun ON mun.muncod=cq.muncod 
			INNER JOIN projovemcampo.cursoofertado cof ON cof.cofid=cq.cofid 
			WHERE qprid='".$_SESSION['projovemcampo']['qprid']."'";
	
	$cabecalho = array("&nbsp;","Município","Cursos");
	$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
	
}

function excluirTurma($dados) {
	global $db;

	$sqlpimid = "SELECT 
					pimid
				FROM
					projovemcampo.planodeimplementacao
				WHERE
					apcid = {$_SESSION['projovemcampo']['apcid']}";
	$pimid = $db->pegaUm($sqlpimid);
	
	$db->executar ( "DELETE FROM projovemcampo.planoprofissional WHERE pimid = {$pimid}" );

	$db->commit ();
	$db->executar ( "UPDATE projovemcampo.turma SET turstatus='I' WHERE turid='" . $dados ['turid'] . "'" );
	$db->commit ();
	$db->executar ( "UPDATE projovemcampo.estudante SET eststatus='I' WHERE turid='" . $dados ['turid'] . "'" );
	$db->commit ();

	$link = "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&aba=turma";
	
	echo "<script>
			alert('Turma excluida com sucesso.');
			window.location='$link';
		  </script>";
}

function recuperaSecretariaPorUfMuncod()
{
	global $db;
	
	if($_SESSION['projovemcampo']['estuf']){
		$stCampo = '';
		$stInner = '';
		//$stWhere = "AND fen.funid = 25 AND fen2.funid = 6 AND ende.estuf = '{$_SESSION['projovemcampo']['estuf']}'";
                $stWhere = "AND fen.funid = 6 AND ende.estuf = '{$_SESSION['projovemcampo']['estuf']}'";
	}else{
		$stCampo = "mun.mundescricao, mun.estuf,";
		$stInner = "INNER JOIN territorios.municipio mun on mun.muncod = ende.muncod";
		//$stWhere = "AND fen.funid = 15 AND fen2.funid = 7 AND mun.muncod = '{$_SESSION['projovemcampo']['muncod']}'";
                $stWhere = "AND fen.funid = 7 AND mun.muncod = '{$_SESSION['projovemcampo']['muncod']}'";
	}
	
	$sql = "
            SELECT  DISTINCT ent.entnome, 
                    ent.entnumcpfcnpj, 
                    ende.endlog, 
                    ende.endcep, 
                    ende.endnum, 
                    ende.endbai,
                    {$stCampo}
                    ent.entnumcpfcnpj as cpfsecretario, 
                    ent.entnome as secretario
            FROM entidade.entidade ent
            
            LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid AND fen.fuestatus = 'A'
            LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                        
            --INNER JOIN entidade.entidade ent2 ON ent2.entid = fea.entid
            --INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = ent2.entid AND fen2.fuestatus = 'A'

            INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
            {$stInner}
            WHERE ent.entstatus = 'A' AND ent.entstatus = 'A' AND ende.endstatus = 'A'
            
            --AND trim(ent.entnumcpfcnpj) IS NOT NULL
            {$stWhere}";
            
	$rsSecretaria = $db->pegaLinha($sql);
	return $rsSecretaria;
}

function recuperaCNPJPrefeitura()
{
	global $db;

	$sql = "
	SELECT  DISTINCT 
		ent.entnumcpfcnpj
		FROM entidade.entidade ent
	
		LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid AND fen.fuestatus = 'A'
		LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
	
		INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
		INNER JOIN territorios.municipio mun on mun.muncod = ende.muncod
		
		WHERE ent.entstatus = 'A' AND ent.entstatus = 'A' AND ende.endstatus = 'A'
		AND fen.funid = 1 AND mun.muncod = '{$_SESSION['projovemcampo']['muncod']}' limit 1";

	$cnpj = $db->pegaUm($sql);
	return $cnpj;
}		
		

function testaMetaPrograma($metvalor, $tpmid){

	global $db;
	
	$sql = "SELECT
				metvalor as valor,
				mtpid as id
			FROM
				projovemcampo.metasdoprograma
			WHERE
				tpmid = $tpmid
				AND ppuid = {$_SESSION['projovemcampo']['ppuid']}
				AND apcid = {$_SESSION['projovemcampo']['apcid']}";
	
	$valor = $db->pegaLinha($sql);
	
	if( $valor['valor'] == '' ){
		$sql = "INSERT INTO projovemcampo.metasdoprograma(tpmid, ppuid, apcid, metvalor) 
				VALUES ($tpmid, {$_SESSION['projovemcampo']['ppuid']}, {$_SESSION['projovemcampo']['apcid']}, $metvalor);";
	}elseif( $valor['valor'] != $metvalor ){
		$sql = "UPDATE projovemcampo.metasdoprograma SET
					metvalor = $metvalor
				WHERE
					tpmid = $tpmid
					AND ppuid = {$_SESSION['projovemcampo']['ppuid']}
					AND apcid = {$_SESSION['projovemcampo']['apcid']};";
	}
	
	if( $sql != '' ){
		$db->executar($sql);
		$db->commit();
	}

}

function recuperaMetasPorUfMuncod($us) {
    global $db;

    if ($_SESSION['projovemcampo']['muncod']) {
        $cmecodibge = " and apccodibge = '" . $_SESSION['projovemcampo']['muncod'] . "'";
    } else {
        $cmecodibge = " and apccodibge = (select estcod from territorios.estado where estuf = '" . $_SESSION['projovemcampo']['estuf'] . "')::numeric";
    }

    $sql = "
        select * from projovemcampo.adesaoprojovemcampo c			
        left join projovemcampo.meta m on c.apcid = m.apcid
        where 
        	c.apcid = {$_SESSION['projovemcampo']['apcid']}			
        {$cmecodibge}
    ";
    $rsMetas = $db->carregar($sql);

    $rs['metatotal'] = $rsMetas[0]['cmemeta'];

    return $rs;
}


function podeMostrarTermosMetas( $dados = array() ){
    global $db;
    
    if ($_SESSION['projovemcampo']['muncod']){

        $sql = "
            SELECT  suametasugerida,
                    suametaajustada
            FROM projovemcampo.adesaoprojovemcampo p
            JOIN projovemcampo.sugestaoampliacao a on a.apcid = p.apcid
            WHERE p.ppuid = {$_SESSION['projovemcampo']['ppuid']} AND p.apcid = {$_SESSION['projovemcampo']['apcid']} AND muncod = '{$_SESSION['projovemcampo']['muncod']}'
        ";
        $rsSugerida = $db->pegaLinha($sql);

        if ($dados['ajustado'] == true) {
            $stNomeCampo = 'suametaajustada';
        }

        if ($dados['sugerido'] == true) {
            $stNomeCampo = 'suametasugerida';
        }

        if ($rsSugerida[$stNomeCampo] > 0) {
            return true;
        } else if (!$dados['sugerido'] && !$dados['ajustado']) {
            return true;
        }
    } else {
        $stWhere = '';
        $stInner = '';
        if ( !$dados['sugerido'] && !$dados['ajustado'] ) {
            $stWhere .= "AND m.tpmid in (7, 10, 13) ";
            $stInner .= "
                JOIN territorios.estado est on est.estuf = p.estuf
                JOIN projovemcampo.cargameta cme on est.estcod::numeric = cme.cmecodibge
                JOIN projovemcampo.metasdoprograma m on cme.cmeid = m.cmeid
            ";
        } else {
            if ($dados['ajustado'] == true){
                $stWhere .= " AND m.tpmid in (9, 12, 15) --ajustados ";
            }

            if ($dados['sugerido'] == true) {
                $stWhere .= " AND m.tpmid in (8, 11, 14) --sugeridas ";
            }

            $stInner .= "
                JOIN projovemcampo.sugestaoampliacao a on a.apcid = p.apcid
                JOIN projovemcampo.metasdoprograma m on a.suaid = m.suaid
            ";
        }
        
        if( $_SESSION['projovemcampo']['estuf'] && $_SESSION['projovemcampo']['ppuid'] == '1' ){
            $sql = "
                SELECT	cmemeta
                FROM projovemcampo.cargameta cme
                JOIN territorios.estado est on cast(est.estcod as integer) = cast(cme.cmecodibge as integer)
                WHERE cme.ppuid = {$_SESSION['projovemcampo']['ppuid']} 
                AND est.estuf = '{$_SESSION['projovemcampo']['estuf']}' 
                AND cmemeta IS NOT NULL
            ";
        }else{
            $sql = "
                SELECT  metvalor
                FROM projovemcampo.adesaoprojovemcampo p
                {$stInner}
                WHERE p.ppuid = {$_SESSION['projovemcampo']['ppuid']} AND m.ppuid = {$_SESSION['projovemcampo']['ppuid']} AND p.apcid = {$_SESSION['projovemcampo']['apcid']} AND p.estuf = '{$_SESSION['projovemcampo']['estuf']}'
                {$stWhere}
                AND metvalor IS NOT NULL
            ";            
        }
        $rsSugeridas = $db->carregar($sql);

        if( $rsSugeridas[0]['cmemeta'] != '' && $_SESSION['projovemcampo']['estuf'] && $_SESSION['projovemcampo']['ppuid'] == '1' ){
            return true;
        }elseif (count($rsSugeridas) == 3) {
            return true;
        }
        return false;
    }
}

    function verificaMetaDestinada( $dados ){
        global $db;

        extract($dados);

        if( $cmeid != '' || $suaid != '' ){
            if(trim($metaDestinada) == "atendida"){
                $sql = "
                    Select tpmid, metvalor From projovemcampo.metasdoprograma Where ppuid = ".$_SESSION['projovemcampo']['ppuid']." and cmeid = ".$cmeid."
                ";
                $meta = $db->carregar($sql);
                $i = 0;
                foreach( $meta as $k => $a){
                    if( $meta[$i]['metvalor'] != 0 && $meta[$i]['tpmid'] == 7){
                        $tipo_meta = 7;
                    }elseif( $meta[$i]['metvalor'] != 0 && $meta[$i]['tpmid'] == 13){
                        $tipo_meta = 13;
                    }
                    $i = $i + 1;
                }
            }
            
            if(trim($metaDestinada) == "sugerida"){
                $sql = "
                    Select tpmid, metvalor From projovemcampo.metasdoprograma Where ppuid = ".$_SESSION['projovemcampo']['ppuid']." and suaid = ".$suaid."
                ";
                $meta = $db->carregar($sql);
                $i = 0;
                if(is_array($meta)){
	                foreach( $meta as $k => $a){
	                    if( $meta[$i]['metvalor'] != 0 && $meta[$i]['tpmid'] == 8){
	                        $tipo_meta = 8;
	                    }elseif( $meta[$i]['metvalor'] != 0 && $meta[$i]['tpmid'] == 14){
	                        $tipo_meta = 14;
	                    }
	                    $i = $i + 1;
	                }
	            }
            }
            
            if(trim($metaDestinada) == "ajustada"){
                $sql = "
                    Select tpmid, metvalor From projovemcampo.metasdoprograma Where ppuid = ".$_SESSION['projovemcampo']['ppuid']." and suaid = ".$suaid."
                ";
                $meta = $db->carregar($sql);
                $i = 0;
                foreach( $meta as $k => $a){
                    if( $meta[$i]['metvalor'] != 0 && $meta[$i]['tpmid'] == 9){
                        $tipo_meta = 9;
                    }elseif( $meta[$i]['metvalor'] != 0 && $meta[$i]['tpmid'] == 15){
                        $tipo_meta = 15;
                    }
                    $i = $i + 1;
                }
            }
            echo $tipo_meta;
        }
    }
    
    
    function carregarSugestaoAmpliacao()
    {
    	global $db;
    
    	$sugestaoampliacao = $db->pegaLinha( "SELECT suaverdade, suametaajustada FROM projovemcampo.sugestaoampliacao WHERE apcid='" . $_SESSION['projovemcampo']['apcid'] . "'" );
    
    	return $sugestaoampliacao;
    }
    
    function carregarMeta($sugestaoampliacao)
    {
    	global $db;
    
    	$sql = "SELECT
			    	metvalor as valor,
			    	mtp.tpmid as tipo
		    	FROM
		    		projovemcampo.metasdoprograma mtp
		    	INNER JOIN projovemcampo.tipometadoprograma tpr ON tpr.tpmid = mtp.tpmid
		    	WHERE
			    	apcid = {$_SESSION['projovemcampo']['apcid']}
			    	AND tprid = {$_SESSION['projovemcampo']['tprid']}
		    	ORDER BY
		    		tipo DESC ";
    	
    	$meta = $db->pegaUm($sql);
//    ver($sql);
    	return $meta;
    }
    
    function gravarTurma($dados) {
    	global $db;
    	extract($dados);
//     	ver($dados,d);
    	if($turid){
    		$sql = "UPDATE projovemcampo.turma
					   SET entid= {$entid},
    					   turqtdalunosprevistos= {$turqtdalunosprevistos}
					 WHERE turid = {$turid};
        			";
    	}ELSE{
		    $sqlsecretaria = "
	        				SELECT
								s.secaid
							FROM
										projovemcampo.secretaria s
							INNER JOIN	projovemcampo.adesaoprojovemcampo a ON a.secaid = s.secaid
							WHERE
								apcid='".$_SESSION['projovemcampo']['apcid']."'";
		    
		    $dadossecretaria = $db->pegaLinha($sqlsecretaria);
		    $sqlqtdturmas = "
					    SELECT
					    	count(turid)
					    FROM
					    			projovemcampo.turma t
					    INNER JOIN	projovemcampo.adesaoprojovemcampo a ON a.secaid = t.secaid
					    WHERE
					    	a.apcid = {$_SESSION['projovemcampo']['apcid']}";
		    $qtdturmas = $db->pegaUm( $sqlqtdturmas );
		    
		    
		    $sql = '';
		    if(!$qtdturmas){
		    	$y = 1;
		    }ELSE{
				$y = $qtdturmas +1;
			}
// 	    	for($x=1;$x<=$nueqtdturma;$x++){
	// 				ver($dados,d);
	    		$sql .= "INSERT INTO projovemcampo.turma( secaid, turdescricao, turstatus, entid,turqtdalunosprevistos) VALUES( ".$dadossecretaria['secaid'].",'Turma $y', 'A', ".$entid.",{$turqtdalunosprevistos} );";
// 	    		$y++;
// 			}
		}
		    $db->executar($sql);
			$db->commit();
		if($turid!=''){		
			echo "<script>
					alert('Turma atualizada com sucesso! A página de Profissionais');
		    					window.location.href = window.location.href
		    			</script>";
	    }else{
	    	echo "<script>
					alert('Turma criada com sucesso!');
		    					window.location.href = window.location.href
		    			</script>";
	    }
    }
    
function pegarEscolas($dados) {
	global $db;
	if($dados) {
		$innerTemAlunos="INNER JOIN projovemcampo.estudante est ON est.turid = tur.turid";
	}
	$perfis = pegaPerfilGeral();
	if(in_array(PFL_COORDENADOR_TURMA, $perfis)){
		$inner_diretor = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.turid = tur.turid AND rpustatus='A'";
	}
	if(in_array(PFL_DIRETOR_ESCOLA, $perfis)){
		$inner_diretor= "INNER JOIN	projovemcampo.usuarioresponsabilidade usu	ON usu.entid = tur.entid AND usucpf = '".$_SESSION['usucpf']."' AND rpustatus='A'";
	}
	$sql = "SELECT
				ent.entid as codigo, 
				ent.entnome as descricao
			FROM entidade.entidade ent
			WHERE entid in (SELECT DISTINCT
								tur.entid
							FROM
								projovemcampo.adesaoprojovemcampo apc
							INNER JOIN	projovemcampo.turma tur ON tur.secaid = apc.secaid
							$innerTemAlunos
							$inner_diretor
							WHERE
								apc.apcid = {$_SESSION['projovemcampo']['apcid']}
							AND tur.turstatus = 'A'
							)";
// 	ver($sql);
	$escolas = $db->carregar ( $sql );
	return $escolas;
} 

function pegarEscolas2($dados){
	global $db;
	
	if($dados['estuf']||$dados['muncod']){
		if($dados['muncod']){
			$sql = "SELECT
						ent.entid as codigo,
						ent.entnome as descricao
					FROM entidade.entidade ent
					INNER JOIN projovemcampo.turma tur ON tur.entid = ent.entid
					INNER JOIN entidade.endereco ende ON ende.entid = tur.entid
					WHERE 
						muncod = '{$dados['muncod']}' ";
// 	ver($dados,d);
		}else{
			$sql = "SELECT
						ent.entid as codigo,
						ent.entnome as descricao
					FROM entidade.entidade ent
					INNER JOIN projovemcampo.turma tur ON tur.entid = ent.entid
					INNER JOIN entidade.endereco ende ON ende.entid = tur.entid
					WHERE 
						estuf =  '{$dados['estuf']}' ";
		}
	}else{
		$sql = array();
	}
// 	ver($dados,d);
		$db->monta_combo('entid', $sql, 'S', 'Selecione', buscarTurmas, '', '', '', 'N', 'entid');
}

function buscarEscolaPorINEP($dados) {
    	global $db;
//     	ver(d);
    
    	$sql = "SELECT
				ent.entid, ent.entnome, tpc.tpcdesc, tpl.tpldesc, ent.entnumdddcomercial, ent.entnumcomercial
			FROM entidade.entidade ent
			LEFT JOIN entidade.tipoclassificacao tpc ON tpc.tpcid=ent.tpcid
			LEFT JOIN entidade.tipolocalizacao tpl ON tpl.tplid=ent.tplid
			WHERE entcodent='".$dados['codinep']."'";
    	$entidade = $db->pegaLinha($sql);
    
    	if($entidade['entid']) {
    
    		$sql = "SELECT ende.endlog, ende.endnum, ende.endcom, ende.endbai, ende.endcep, mun.mundescricao, mun.estuf FROM entidade.endereco ende
				INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
				WHERE entid='".$entidade['entid']."' AND tpeid='1'";
    		$entidade['endereco'] = $db->pegaLinha($sql);
    	}
    
    	echo simec_json_encode($entidade);
    
}
    
    function montarCombosMunicipioRede($dados) {
    	global $db;
    	?>
    	<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
    		<tr>
    			<td class="SubTituloDireita">Município</td>
    			<td><? $db->monta_combo('muncod', "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$dados['estuf']."' ORDER BY mundescricao", (($_SESSION['projovemcampo']['muncod'])?'N':'S'), 'Selecione', '', '', '', '', 'S', 'muncod', '', $_SESSION['projovemcampo']['muncod']); ?></td>
    		</tr>
    		<tr>
    			<td class="SubTituloDireita">Rede</td>
    			<td>
    			<?
    			$op[] = array('codigo'=>'1','descricao'=>'FEDERAL');
    			$op[] = array('codigo'=>'2','descricao'=>'ESTADUAL');
    			$op[] = array('codigo'=>'3','descricao'=>'MUNICIPAL');
    			$op[] = array('codigo'=>'4','descricao'=>'PRIVADA'); 
    			$db->monta_combo('id_dependencia_adm',$op , 'S', 'Selecione', '', '', '', '', 'S', 'id_dependencia_adm', ''); 
    			?></td>
    		</tr>
                    <tr>
                      <td class="SubTituloDireita">Nome</td>
                      <td>
                        <?php echo campo_texto('entnome', 'N', 'S', '', 20, 50, '', '', '', '', null, 'id="entnome"'); ?>
                        <input type="hidden" name="escolatipo" value="<?php echo $dados['escolatipo']; ?>" />
                      </td>
                    </tr>
    		<tr>
    			<td colspan="2" class="SubTituloCentro"><input type="button" value="Pesquisar" onclick="pesquisarEscolas(document.getElementById('muncod').value,document.getElementById('id_dependencia_adm').value,document.getElementById('entnome').value);"></td>
    		</tr>
    		<tr>
    			<td colspan="2">
    			<div id="div_escolas" style="position:absolute;left:6px;margin:13px;width:415px;height:180px;overflow:auto"></div>
    			</td>
    		</tr>
    	</table>
    	<?
    }
    
    function buscarEscolas($dados) {
    	global $db;
    	?>
    	<html>
    	<body>
    	<script>
    	function selecionarMunicipio(estuf) {
    		if(estuf) {
    		
    	 		document.getElementById('tr_filtros').style.display='';
    			jQuery.ajax({
    		   		type: "POST",
    		   		url: "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A",
    		   		data: "requisicao=montarCombosMunicipioRede&estuf="+estuf+'&escolatipo='+'<?php echo $dados['escolatipo']; ?>',
    		   		async: false,
    		   		success: function(msg){document.getElementById('td_filtros').innerHTML=msg;}
    		 		});
    	 		
    	 	} else {
    	 	
    	 		document.getElementById('tr_filtros').style.display='none';
    	 		document.getElementById('td_filtros').innerHTML='Carregando...';
    	 		
    	 	}
    	}
    	
    	function pesquisarEscolas(muncod, id_dependencia_adm, entnome) {
    	
    		if(muncod=='') {
    			alert('Selecione um Município');
    			return false;
    		}
    	
    		if(id_dependencia_adm=='') {
    			alert('Selecione uma Rede');
    			return false;
    		}
    
    		jQuery.ajax({
    	   		type: "POST",
    	   		url: "projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A",
    	   		data: "requisicao=pesquisarEscolas&muncod="+muncod+"&id_dependencia_adm="+id_dependencia_adm+"&entnome="+entnome,
    	   		async: false,
    	   		success: function(msg){
    	   			document.getElementById('div_escolas').innerHTML=msg;
    	   			}
    	 		});
    
    	}
    	
    	function marcarCodigoInep(codinep) {
    		document.getElementById('entcodent<?=$dados['escolatipo'] ?>').value=codinep;
    		document.getElementById('entcodent<?=$dados['escolatipo'] ?>').onblur();
    		closeMessage();
    	}
    
    	</script>
    	<?
    	if($_SESSION['projovemcampo']['estuf']){
    		$estuf = $_SESSION['projovemcampo']['estuf'];
    	}
    	if($_SESSION['projovemcampo']['muncod']){
    		$estuf = $db->pegaUm("SELECT estuf FROM territorios.municipio WHERE muncod='".$_SESSION['projovemcampo']['muncod']."'");
    	}
    	if($estuf) :
    	?>
    	<script>
    	jQuery(document).ready(function() {
    		selecionarMunicipio('<?=$estuf ?>');
    	});
    	</script>
    	<?
    	endif;
    	?>
    	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
    		<tr>
    			<td class="SubTituloDireita">UF</td>
    			<td><? $db->monta_combo('estuf', "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado", (($estuf)?'N':'S') , 'Selecione', 'selecionarMunicipio', '', '', '', 'S', '', '', $estuf); ?>  <input type="button" value="Fechar" onclick="closeMessage();"></td>
    		</tr>
    		<tr style="display:none;" id="tr_filtros">
    			<td id="td_filtros" colspan="2"></td>
    		</tr>
    	</table>
    	</body>
    	<?
    	
    }
    
function verificaCadastroCPF( $dados ){
    
global $db;

	$sql = "SELECT DISTINCT
				estid
			FROM
				projovemcampo.estudante
			WHERE
				estcpf = '".$dados['cpf']."'";
	$teste = $db->pegaUm($sql);
	
	$sql = "
			SELECT
				caeid
			FROM
			projovemurbano.cadastroestudante
			WHERE
				caecpf = '".$dados['cpf']."'
			AND caestatus = 'A'";
	$participou = $db->pegaUm($sql);
	
	if($teste != ''){
		echo 1;
	}elseif($participou !=''){
		echo 2;
	}else{
		echo  0;
	}
// 	echo $teste == 't' ? 'true' : 'false';
    	//	return $teste == 't' ? 'true' : 'false';
    die;
}

function buscarTurmas($dados) {
	global $db;
	$perfis = pegaPerfilGeral();
	if(in_array(PFL_DIRETOR_ESCOLA, $perfis)){
		$inner_escolas = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.entid = tur.entid AND rpustatus='A'";
	}
	if($_SESSION['projovemcampo']['apcid']){
		$apcid = "AND apcid = {$_SESSION['projovemcampo']['apcid']}";
	}
	if($dados['alunos']=='t'){
		$innercomalunos = "INNER JOIN projovemcampo.estudante est ON est.turid = tur.turid";
	}else{
// 		$whereAlunos = "AND tur.turqtdalunosprevistos > (
// 												SELECT
// 													count(*)
// 												FROM projovemcampo.estudante c
// 												WHERE
// 													c.turid = tur.turid
// 													AND eststatus = 'A'
// 												)";
	}
		$sql = "SELECT DISTINCT
					tur.turid as codigo,
					tur.turdescricao ||', Total de Alunos: '||(
															SELECT 
																count(*) 
															FROM projovemcampo.estudante c 
															WHERE 
																c.turid = tur.turid 
																AND eststatus = 'A'
															) 
					as descricao
				FROM
				projovemcampo.turma tur
				--INNER JOIN	projovemcampo.estudante est ON est.turid = tur.turid
				INNER JOIN	projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
				$innercomalunos
				$inner_escolas
				WHERE
					tur.entid = {$dados['entid']}
				AND tur.turstatus = 'A'
				$whereAlunos
				$apcid
				ORDER BY
					tur.turid";
// 	ver($sql,d);
	$dados['bloq'] = $dados['bloq'] ? $dados['bloq'] : 'S';
	if($dados['form']=='M'){
		$db->monta_combo('turidM', $sql, $dados['bloq'], 'Selecione uma Turma', '', '', '', '', 'N', 'turidM');
	}else{
		$db->monta_combo('turid', $sql, $dados['bloq'], 'Selecione uma Turma', '', '', '', '', 'N', 'turid');
	}
}

function buscarTurmas2($dados) {
	global $db;
	$perfis = pegaPerfilGeral();
	if(in_array(PFL_DIRETOR_ESCOLA, $perfis)){
		$inner_escolas = "INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.usucpf='".$_SESSION['usucpf']."' AND ur.entid = tur.entid AND rpustatus='A'";
	}
	
	$sql = "SELECT
				tur.turid as codigo,
				tur.turdescricao as descricao--', Total de Alunos: '||(SELECT count(*) FROM projovemcampo.estudante c WHERE c.turid = tur.turid AND eststatus = 'A') as descricao
			FROM
						projovemcampo.turma tur
			INNER JOIN	projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
			$inner_escolas
			WHERE
				apc.apcid = {$_SESSION['projovemcampo']['apcid']}
			AND tur.turstatus = 'A'
			ORDER BY
					tur.turid";
	$dados = is_array($dados) ? $dados : array();
	$dados['bloq'] = $dados['bloq'] ? $dados['bloq'] : 'S';
	$db->monta_combo('turid2', $sql, $dados['bloq'], 'Selecione uma Turma', '', '', '', '', 'N', 'turid2');
}

function testaQtdAlunoTurma( $dados ){
	global $db;
	$sqlEstudante = '';

	if( is_array($dados) ){
		$turid        = $dados['turid'];
		$sqlEstudante = " AND est.estcpf <> '".$dados['cpfestudante']."'";
	}else{
		$turid = $dados;
	}

	if( $turid && $turid != 'undefined' && is_numeric($turid)){
		$sql = "SELECT
						true
				FROM
				(
					SELECT
						count(estid) as qtd,
						tur.turqtdalunosprevistos
					FROM
						projovemcampo.turma tur
					INNER JOIN projovemcampo.estudante est ON est.turid = tur.turid AND est.eststatus = 'A'
					WHERE tur.turid = ".$turid."
					{$sqlEstudante}
					group By
						tur.turqtdalunosprevistos
					) as foo
					WHERE
						qtd >= turqtdalunosprevistos";
					$boolean = $db->pegaUm($sql);
	}
	return ($boolean == 't' ? true : false);
}

function testaQtdAlunoMetaProjovem( $apcid ){

	global $db;
	$sqlmeta = "SELECT true FROM projovemcampo.meta WHERE apcid = {$_SESSION['projovemcampo']['apcid']} AND tpmid = 3";
	$temAjuste = $db->pegaUm($sqlmeta);
	if($temAjuste == 't'){
		$sql="
				SELECT
					true
				FROM
				(
					SELECT DISTINCT
						apcid,
						sum(metvalor) as qtd
					FROM projovemcampo.meta m
					WHERE
						tpmid = 3
					AND apcid = {$_SESSION['projovemcampo']['apcid']}
					GROUP BY
					m.apcid
				) as foo
				WHERE
					foo.qtd <= (
							SELECT 
								count(estid) 
							FROM projovemcampo.estudante est 
							INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
							INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
							WHERE 
								apcid = foo.apcid AND eststatus = 'A'  )
				AND foo.apcid = {$_SESSION['projovemcampo']['apcid']} ";
		$boolean = $db->pegaUm($sql);
	}else{
	$sql="
			SELECT
					true
				FROM
				(
					SELECT DISTINCT
						apcid,
						sum(metvalor) as qtd
					FROM projovemcampo.meta m
					WHERE
						tpmid = 1
					AND apcid = {$_SESSION['projovemcampo']['apcid']}
					GROUP BY
					m.apcid
				) as foo
				WHERE
					foo.qtd <= (
							SELECT 
								count(estid) 
							FROM projovemcampo.estudante est 
							INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
							INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
							WHERE 
								apcid = foo.apcid AND eststatus = 'A'  )
				AND foo.apcid = {$_SESSION['projovemcampo']['apcid']}";
	$boolean = $db->pegaUm($sql);
	}
	//       ver($sql,d);
	return ($boolean == 't' ? true : false);
}

function historicoCadastro($estid, $usucpf,$hictipo, $hicacao) {
	global $db;

	#@ hicid - id da tabela historicocadastro
	#@ estid - id da tabela estudante
	#@ usucpf - CPF do estudante
	#@ hicdataacao - data da acao
	#@ hictipo Tipo de Status: a - ativação, i -  inativação
	#@ hicacao Tipo da ação realizada: Insert - Update - Delete
	#@ hicstatus Status da ação-sistema por DEFAULT é sempre 'A'
	$sql = "
			Insert Into projovemcampo.historicocadastro(estid,usucpf,hicdataacao,hictipo,hicacao)
			Values($estid,'$usucpf','now()','$hictipo','$hicacao');
	";
	if( $sql != '' ){
	$db->executar($sql);
	}
$db->commit();
}
function enderecoEscola($dados) {
	global $db;
	$sql = "
			SELECT 
				no_entidade, 
				num_cep, 
				desc_endereco,
		        num_endereco, 
		        desc_endereco_complemento,
		        desc_endereco_bairro, '(' || num_ddd || ') '|| num_telefone AS num_telefone, mun.mundescricao
		  	FROM educacenso_2013.tab_entidade tent
		    INNER JOIN territorios.municipio mun ON (muncod::int = fk_cod_municipio)
		  	WHERE tent.pk_cod_entidade = {$dados['entid']}
		";
	$dadosEntidade = $db->pegaLinha ( $sql );
	?>
<html>
<head>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
</head>
<body>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
		align="center">
		<tr>
			<td class="SubTituloDireita" width="30%">Nome da escola</td>
			<td><?php echo $dadosEntidade['no_entidade']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Código INEP</td>
			<td><?php echo $dados['entid']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Telefone</td>
			<td><?php echo $dadosEntidade['num_telefone']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Endereço</td>
			<td><?php echo $dadosEntidade['desc_endereco']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Número</td>
			<td><?php echo $dadosEntidade['num_endereco']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Complemento</td>
			<td><?php echo $dadosEntidade['desc_endereco_complemento']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Bairro</td>
			<td><?php echo $dadosEntidade['desc_endereco_bairro']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">CEP</td>
			<td><?php echo $dadosEntidade['num_cep']; ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="30%">Município</td>
			<td><?php echo $dadosEntidade['mundescricao']; ?></td>
		</tr>
		<tr>
			<td colspan="2" align="center"><button onclick="window.close();">Fechar</button></td>
		</tr>
	</table>
</body>
</html>

<?

}
function verificaCadastroCPF2( $dados ){
	global $db;
	
	$sql = "SELECT
				1
			FROM
				projovemcampo.estudante
			WHERE
				estcpf = '".str_replace(Array('.','-'),'',$dados['estcpf'])."'";
	$teste = $db->pegaUm($sql);
	
	$sql= "
			SELECT
				2
			FROM
				projovemurbano.cadastroestudante
			WHERE
				caecpf = '".str_replace(Array('.','-'),'',$dados['estcpf'])."'
			AND caestatus = 'A'";
	$teste = $db->pegaUm($sql);
	
// 	$teste == '1' ? 'true' : 'false';
	
	if($teste == 'true'){
		return $teste ;
	}else{
		return $teste;
	}
}
// function verificaCertificadoTotalBolsa($dados) {
// 	global $db;

// 	$sql = "SELECT dbucertificado
// 			FROM projovemcampo.dadosbatimentoum
// 			WHERE
// 				dbucpf='" . str_replace ( array (// 						".",
// 						"-"
// 				), array (
// 						"",
// 						""
// 				), $dados ['cpf'] ) . "'
// 				AND (dbuprojeto='PROJOVEM_URBANO' OR dbuprojeto='PROJOVEM_ORIGINAL')
// 				AND dbucertificado > 0";
// 	$dbucertificado = $db->pegaUm ( $sql );

// 	if (! $dbucertificado && verifica_data ( $dados ['caedatanasc'] )) {
// 		$sql = "SELECT dbucertificado FROM projovemcampo.dadosbatimentoum
// 				WHERE
// 					UPPER(dbunomeestudante)=UPPER('" . removeacentos ( trim ( $dados ['caenome'] ) ) . "')
// 					AND UPPER(dbunomemae)=UPPER('" . removeacentos ( trim ( $dados ['caenomemae'] ) ) . "')
// 					AND dbudatanasc='" . formata_data_sql ( $dados ['caedatanasc'] ) . "'
// 					AND (dbuprojeto='PROJOVEM_URBANO' OR dbuprojeto='PROJOVEM_ORIGINAL')
// 					AND dbucertificado > 0";

// 		$dbucertificado = $db->pegaUm ( $sql );
// 	}

// 	$sql = "SELECT dbutotalbolsas FROM projovemcampo.dadosbatimentoum WHERE dbucpf='" . str_replace ( array (
// 			".",
// 			"-"
// 	), array (
// 			"",
// 			""
// 	), $dados ['cpf'] ) . "' AND dbuprojeto='PROJOVEM_URBANO'";
// 	$caeqtddireitobolsa = $db->pegaUm ( $sql );

// 	if (! $caeqtddireitobolsa && verifica_data ( $dados ['caedatanasc'] )) {
// 		$sql = "SELECT dbutotalbolsas
// 				FROM projovemcampo.dadosbatimentoum
// 				WHERE
// 					UPPER(dbunomeestudante)=UPPER('" . removeacentos ( trim ( $dados ['caenome'] ) ) . "') AND
// 					UPPER(dbunomemae)=UPPER('" . removeacentos ( trim ( $dados ['caenomemae'] ) ) . "') AND
// 					dbudatanasc='" . formata_data_sql ( $dados ['caedatanasc'] ) . "' AND
// 					dbuprojeto='PROJOVEM_URBANO'";
// 		$caeqtddireitobolsa = $db->pegaUm ( $sql );
// 	}
// 	// ver($caeqtddireitobolsa,d);
// 	if ($caeqtddireitobolsa == '' && $dbucertificado == '') {
// 		$totalbolsa = '18';
// 	} else {
// 		$totalbolsa = (($caeqtddireitobolsa > 18) ? 0 : 18 - $caeqtddireitobolsa);
// 		$totalbolsa = $dbucertificado ? '0' : $totalbolsa;
// 	}
// 	echo $dbucertificado . ";" . $totalbolsa;
// }

function inserirEstudantes($dados) {
	global $db;
	
	$dados['sesid'] = $dados['sesid'] ? "'".$dados['sesid']."'" : 'NULL';
	$dados['estegressobrasilalfabetizado'] = $dados['estegressobrasilalfabetizado'] ? $dados['estegressobrasilalfabetizado'] : 'FALSE';
	$dados['cpf'] = str_replace(Array('.','-'),'',$dados['estcpf']);
	$dados['esttelefone'] = str_replace('-', '', $dados['esttelefone']);
	$dados['estcelular'] = str_replace('-', '', $dados['estcelular']);
	$dados['estnomemae'] = empty($dados['estnomemae'])?'IGNORADA':$dados['estnomemae'];
	$dados['estnomepai'] = empty($dados['estnomepai'])?'IGNORADO':$dados['estnomepai'];
	$dados['qtdfilhos']	= $dados['qtdfilhos'] < 1 ? '0' : $dados['qtdfilhos'];
// 	$dados['estendregiaomoradia'] = 'TRUE' ? 'u':'r';
	
	$obrigatorios = Array('corid','escid','sesid','estendestuf','estendmuncod','estnome','estnomemae','estnaturalidade','esttemdeficiencia','estfilhos','estbeneficiariooutroprograma','turid',
			'estnumrg','estorgaoexpedidorg','estdataemissaorg','estendcep','estendlogradouro','estendnumero','estendbairro','esttelefone','eststatus','estaltashabilidades',
			'estegressobrasilalfabetizado','estestufemissao');

	$sql = "SELECT
				TRUE
			FROM
				projovemcampo.estudante
			WHERE
				estcpf = '{$dados['cpf']}'";
	$testecadastro = $db->pegaUm ( $sql );
	// -- URL de redirecionamento após o processamento
	
			$urlestudantes	= 'projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes';
			 
		foreach ($obrigatorios as $obrigatorio) {

			if( $dados[$obrigatorio] == '' ){
				die("<script>
					alert('Erro na validação das informações.');
					window.location='{$urlestudantes}';
					</script>");
			}
		}

	if($testecadastro!=true){
		
		/*Regra exigida por wallace - 25/06/2012*/
		
		if( testaQtdAlunoMetaProjovem( $_SESSION['projovemcampo']['apcid']) ){
			die("<script>
				alert('Meta atingida.');
				window.location='{$urlestudantes}';
				</script>");
		}
				
		/*FIm Regra exigida por wallace - 25/06/2012*/
				
		if( testaQtdAlunoTurma( $dados['turid'] ) ){
		die("<script>
		alert('Turma lotada. Escolha outra.');
				window.location='{$urlestudantes}';
				</script>");
		}
		/*Fim Regra exigida por wallace - 08/05/2012*/
		$teste = verificaCadastroCPF2( $dados );
		if( $teste == '1' ){

			die("<script>
				alert('CPF já cadastrado.');
				window.location='{$urlestudantes}';
			</script>");

		}elseif($teste == '2'){
			die("<script>
					alert('CPF já cadastrado no Projovem Urbano.');
					window.location='{$urlestudantes}';
			</script>");
		}
		$dados['eststatus'] = $dados['eststatus'] ? $dados['eststatus'] : 'A';
		
// 		ver($dados['estnumrg'],
// 	                            $dados['estorgaoexpedidorg'],
// 	                            $dados['estestufemissao'],
// 			            		$dados['estdataemissaorg'],d);
		$sql = "INSERT INTO projovemcampo.estudante(
	                                                corid,
										            defid,
	                                                trdid,
	                                                escid,
	                                                bprid,
	                                                ocuid,
	                                                sesid,
	                                                estendestuf,
	                                                estendmuncod,
	                                                estcpf,
	                                                estnome,
	                                                estdatanascimento,
	                                                estsexo,
	                                                estnomemae,
	                                                estnomepai,
	                                                estnaturalidade, -- grava nacionalidade
	                                                estestufnasc,
	                                                estmuncodnasc,
	                                                estnis,
	                                                esttemdeficiencia,
	                                                estfilhos,
	                                                estbeneficiariooutroprograma,
	                                                estocupacao,
	                                                estegressobrasilalfabetizado,
	                                                esthistoricoescolar,
	                                                esttesteproficiencia,
	                                                turid,
	                                                estauxiliosareceber,
	                                                estnumrg,
	                                                estorgaoexpedidorg,
	                                                estestufemissao,
	                                                estdataemissaorg,
	                                                estendcep,
	                                                estendlogradouro,
	                                                estendnumero,
	                                                estendcomplemento,
	                                                estendbairro,
	                                                estemail,
	                                                esttelefone,
	                                                estcelular,
	                                                eststatus,
	                                                estaltashabilidades,
	                                                estmotivoinativacao,
	                                                egressooutroprogramaalfabetizac,
	                                                -- turno,
	                                                minid,
	                                                estcumpremedidassocioeducativas,
	                                                estparticipouprojovemcampo,
													estendregiaomoradia)
						VALUES (
	    						'".$dados['corid']."',
			    				".(($dados['defid'])?"'".$dados['defid']."'":"NULL").",
			    				".(($dados['trdid'])?"'".$dados['trdid']."'":"NULL").",
			    				'".$dados['escid']."',
			    				".(($dados['bprid'])?"'".$dados['bprid']."'":"NULL").",
			    				".(($dados['ocuid'])?"'".$dados['ocuid']."'":"NULL").",
			    				{$dados['sesid']},
			    				'".$dados['estendestuf']."',
			    				'".$dados['estendmuncod']."',
			    				'".str_replace(array(".","-"),"",$dados['estcpf'])."',
	                            '".$dados['estnome']."',
	                            '".formata_data_sql($dados['estdatanascimento'])."',
	                            '".$dados['estsexo']."',
	                            '".$dados['estnomemae']."',
	                            ".(($dados['estnomepai'])?"'".$dados['estnomepai']."'":"NULL").",
	                            '".$dados['estnaturalidade']."', -- grava nacionalidade
	                            '".$dados['estestufnasc']."',
	                            '".$dados['estmuncodnasc']."',
	                            ".(($dados['estnis'])?"'".$dados['estnis']."'":"NULL").",
	                            ".$dados['esttemdeficiencia'].",
	                            ".$dados['estfilhos'].",
	                            ".$dados['estbeneficiariooutroprograma'].",
			            		".(($dados['estocupacao'])?"'".$dados['estocupacao']."'":"NULL").",
	                            ".$dados['estegressobrasilalfabetizado'].",
	                            ".$dados['esthistoricoescolar'].",
	                            ".$dados['esttesteproficiencia'].",
	                            ".$dados['turid'].",
			            		".((is_numeric($dados['estauxiliosareceber']))?"'".$dados['estauxiliosareceber']."'":"NULL").",
	                            '".$dados['estnumrg']."',
	                            '".$dados['estorgaoexpedidorg']."',
	                            '".$dados['estestufemissao']."',
			            		'".formata_data_sql($dados['estdataemissaorg'])."',
	                            '".str_replace(array("-"),array(""),$dados['estendcep'])."',
	                            '".$dados['estendlogradouro']."',
			            		'".$dados['estendnumero']."',
	                            ".(($dados['estendcomplemento'])?"'".$dados['estendcomplemento']."'":"NULL").",
	                            '".$dados['estendbairro']."',
	                            ".(($dados['estemail'])?"'".$dados['estemail']."'":"NULL").",
	                            '".$dados['esttelefone']."',
	                            ".(($dados['estcelular'])?"'".$dados['estcelular']."'":"NULL").",
			            		'".$dados['eststatus']."',
	                            ".$dados['estaltashabilidades'].",
	                            ".($dados['estmotivoinativacao']!=''?"'".$dados['estmotivoinativacao']."'":"null").",
	                            ".$dados['egressooutroprogramaalfabetizac'].",
	                         --   '".$dados['turno']."',
	                            ".($dados['minid']!=''?"'".$dados['minid']."'":"null").",
	                            ".(($dados['estcumpremedidassocioeducativas'])?"'".$dados['estcumpremedidassocioeducativas']."'":"NULL").",
	                            ".(($dados['estparticipouprojovemcampo'])?"'".$dados['estparticipouprojovemcampo']."'":"NULL").",
			    				'".$dados['estendregiaomoradia']."') RETURNING estid;";
	    $estid  = $db->pegaUm($sql);
	    
		$sql = '';
		
		if($dados['estfilhos'] == 'TRUE') {
			foreach($dados['estqtdfilhos'] as $idfid => $qtdfilhos) {
				if($qtdfilhos) {
					$sql .= "INSERT INTO projovemcampo.estudantefaixaetariafilhos(fefid, estid, qtdfilhos)
							VALUES ({$idfid},{$estid}, ".trim($qtdfilhos).");";
				}
			}
		}
	
		if($dados['traid']) {
			$sql = "DELETE FROM projovemcampo.estudanterecursoacessibilidade WHERE estid = $estid;";
			foreach($dados['traid'] as $traid) {
				$sql .= "INSERT INTO projovemcampo.estudanterecursoacessibilidade(estid, traid)
						VALUES ({$estid}, $traid);";
			}
		}
// 	ver($dados,$db,d);
		if( $sql != '' ){
			$db->executar($sql);
		}
		$db->commit();

// 	//HISTORICO - paramentros do historico.
	$estid		= $estid;
	$usucpf 	= $_SESSION['usucpf'];
	$usucpf 	= str_replace(array(".","-"),array("",""),$usucpf);
	$hictipo	= $dados['eststatus'];
	$hicacao	= "I";
	historicoCadastro($estid, $usucpf,$hictipo, $hicacao);
// 	//HOSITORICO end.
		echo "<script>
				alert('Estudante inserido com sucesso.');
				window.open( 'projovemcampo.php?modulo=principal/popComprovante&acao=A&estid=".$estid."', 'Comprovante', 'width=480,height=265,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				window.location='{$urlestudantes}';
			 </script>";
		die;
	}else{
		echo "<script>
				window.location='{$urlestudantes}';
			 </script>";
		die;
	}
}

function atualizarEstudantes($dados) {
	global $db;
	// -- URL de redirecionamento após o processamento
	$dados['turid'] = $dados['turid'] ? $dados['turid'] : $dados['turid_bkp'];
	$urlestudantes	= 'projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=estudantes';
	
  	if ($dados['turid'] == '') {
    	echo "<script>
    	alert('Escolha uma turma.');
    	window.location='{$urlestudantes}';
    	</script>";
                            					}

	$teste = verificaCadastroCPF2( $dados );
		if($teste == '2'){
			die("<script>
					alert('CPF já cadastrado no Projovem Urbano.');
					window.location='{$urlestudantes}';
			</script>");
		}
  	if( $teste == 'true' ){
  	
  		die("<script>
  				alert('CPF já cadastrado.');
  				window.location='{$urlestudantes}';
  				</script>");
  	
  	}
  	$dados['corid'] = $dados['corid'] ? $dados['corid'] : 'null';
  	$dados['defid'] = $dados['defid'] ? $dados['defid'] : 'null';
  	$dados['trdid'] = $dados['trdid'] ? $dados['trdid'] : 'null';
  	$dados['escid'] = $dados['escid'] ? $dados['escid'] : 'null';
  	$dados['bprid'] = $dados['bprid'] ? $dados['bprid'] : 'null';
  	$dados['ocuid'] = $dados['ocuid'] ? $dados['ocuid'] : 'null';
  	$dados['sesid'] = $dados['sesid'] ? $dados['sesid'] : 'null';
  	$dados['entid'] = $dados['entid'] ? $dados['entid'] : 'null';
	
	$dados['esttesteproficiencia'] = $dados['esttesteproficiencia'] ? $dados['esttesteproficiencia'] : 'false';
	$status = $dados['eststatus'];
	$dados['eststatus'] = $dados['eststatus'] ? "'".$dados['eststatus']."'" : "'A'"; //'NULL';
	$dados['sesid'] = $dados['sesid'] ? "'".$dados['sesid']."'" : 'null';
  	$dados['esttemdeficiencia'] = $dados['esttemdeficiencia'] ? "'".$dados['esttemdeficiencia']."'" : 'null';
  	$dados['estbeneficiariooutroprograma'] = $dados['estbeneficiariooutroprograma'] ? "'".$dados['estbeneficiariooutroprograma']."'" : 'null';
  	$dados['estocupacao'] = $dados['estocupacao'] ? "'".$dados['estocupacao']."'" : 'null';
  	$dados['estegressobrasilalfabetizado'] = $dados['estegressobrasilalfabetizado'] ? "'".$dados['estegressobrasilalfabetizado']."'" : 'null';
  	$dados['esthistoricoescolar'] = $dados['esthistoricoescolar'] ? "'".$dados['esthistoricoescolar']."'" : 'null';
	$dados['estegressobrasilalfabetizado'] = $dados['estegressobrasilalfabetizado'] ? $dados['estegressobrasilalfabetizado'] : 'FALSE';
	$dados['qtdfilhos']= $dados['qtdfilhos'] < 1 ? '0' : $dados['qtdfilhos'];
// 	$dados['estendregiaomoradia'] = 'TRUE' ? 'u':'r';
// 	ver($dados,d);
	$sql = "UPDATE projovemcampo.estudante
			SET
				corid={$dados['corid']},
				defid={$dados['defid']},
				trdid=".$dados['trdid'].",
				escid=".$dados['escid'].",
				bprid=".$dados['bprid'].",
				ocuid=".$dados['ocuid'].",
				sesid=".$dados['sesid'].",
				estcpf='".str_replace(array(".","-"),"",$dados['estcpf'])."',
				estnome='".$dados['estnome']."',
				estdatanascimento='".formata_data_sql($dados['estdatanascimento'])."',
				estnomemae='".$dados['estnomemae']."',
				estnomepai='".$dados['estnomepai']."',
				estnaturalidade='".$dados['estnaturalidade']."',
				estestufnasc='".$dados['estestufnasc']."',
				estmuncodnasc='".$dados['estmuncodnasc']."',
				estnis='".trim($dados['estnis'])."',
				esttemdeficiencia=".$dados['esttemdeficiencia'].",
				estfilhos=".$dados['estfilhos'].",
				estbeneficiariooutroprograma=".$dados['estbeneficiariooutroprograma'].",
				estocupacao=".$dados['estocupacao'].",
				estegressobrasilalfabetizado=".$dados['estegressobrasilalfabetizado'].",
				esthistoricoescolar=".$dados['esthistoricoescolar'].",
				esttesteproficiencia=".$dados['esttesteproficiencia'].",
				turid=".$dados['turid'].",
				estauxiliosareceber=".(is_numeric($dados['estauxiliosareceber'])?"'".$dados['estauxiliosareceber']."'":"NULL").",
				estnumrg='".$dados['estnumrg']."',
				estorgaoexpedidorg='".$dados['estorgaoexpedidorg']."',
				estestufemissao=".(($dados['estestufemissao'])?"'".$dados['estestufemissao']."'":"NULL").",
				estdataemissaorg='".formata_data_sql($dados['estdataemissaorg'])."',
				estendcep='".str_replace(array("-"),array(""),$dados['estendcep'])."',
				estendlogradouro='".$dados['estendlogradouro']."',
				estendnumero='".$dados['estendnumero']."',
				estendcomplemento=".(($dados['estendcomplemento'])?"'".$dados['estendcomplemento']."'":"NULL").",
				estendbairro='".$dados['estendbairro']."',
				estemail='".$dados['estemail']."',
				esttelefone='".str_replace('-','',$dados['esttelefone'])."',
				estcelular='".str_replace('-','',$dados['estcelular'])."',
				estendestuf='".$dados['estendestuf']."',
				estendmuncod='".$dados['estendmuncod']."',
				estsexo='".$dados['estsexo']."',
				estaltashabilidades = ".$dados['estaltashabilidades'].",
				eststatus = ".$dados['eststatus'].",
				estmotivoinativacao = ".($dados['estmotivoinativacao']!=''?"'".$dados['estmotivoinativacao']."'":"null").",
				egressooutroprogramaalfabetizac = ".$dados['egressooutroprogramaalfabetizac'].",
				      --turno = '".$dados['turno']."',
				minid = ".(($dados['minid'])?"'".$dados['minid']."'":"NULL").",
				estcumpremedidassocioeducativas = ".(($dados['estcumpremedidassocioeducativas'])?"'".$dados['estcumpremedidassocioeducativas']."'":"NULL").",
				estparticipouprojovemcampo=".(($dados['estparticipouprojovemcampo'])?"'".$dados['estparticipouprojovemcampo']."'":"NULL").",
				estendregiaomoradia = '".$dados['estendregiaomoradia']."'
				WHERE estid='".$dados['estid']."';";

	$sql .= "DELETE FROM projovemcampo.estudantefaixaetariafilhos WHERE estid = ".$dados['estid'].";";
	if ($dados['estfilhos'] == 'TRUE') {
			
		foreach($dados['estqtdfilhos'] as $idfid => $qtdfilhos) {
			if($qtdfilhos) {
				
				$sql .= "INSERT INTO projovemcampo.estudantefaixaetariafilhos(fefid, estid, qtdfilhos)
						 VALUES ({$idfid},{$dados['estid']}, ".trim($qtdfilhos).");";
			}
		}
	}
//ver($sql,d);
	$sql .= "DELETE FROM projovemcampo.estudanterecursoacessibilidade WHERE estid='".$dados['estid']."';";
	//$db->executar($sql);
	
	if($dados['traid']) {
		foreach($dados['traid'] as $traid) {
			$sql .= "INSERT INTO projovemcampo.estudanterecursoacessibilidade(estid, traid)
			VALUES ({$dados['estid']}, $traid);";
		}
	}
	//ver($sql,d);
	$db->executar($sql);
	$db->commit();

// 	//HISTORICO - paramentros do historico.
	$estid		= $dados['estid']; //$dados['estid']
	$usucpf 	= $_SESSION['usucpf'];
	$usucpf 	= str_replace(array(".","-"),array("",""),$usucpf);
	$hictipo	= $status;
	$hicacao	= "A";
	
	historicoCadastro($estid, $usucpf,$hictipo, $hicacao);
// 	//HOSITORICO end.

	echo "<script>
			alert('Estudante atualizado com sucesso');
			window.open( 'projovemcampo.php?modulo=principal/popComprovante&acao=A&estid=".$dados['estid']."', 'Comprovante', 'width=480,height=265,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
                        window.location='{$urlestudantes}';
		  </script>";

}
function testacoordturma(){
	global $db;
	
	$sqlcoordturma="SELECT
							true 
						FROM 
							projovemcampo.planoprofissional ppr
						INNER JOIN projovemcampo.planodeimplementacao pim ON pim.pimid = ppr.pimid
						WHERE
							apcid ={$_SESSION['projovemcampo']['apcid']}
						AND profid = 2";
	$temcoordturma = $db->pegaUm ($sqlcoordturma);
	$temcoordturma = $temcoordturma?$temcoordturma:false;
	return $temcoordturma;
}
function adicionaHistoricoDiario($parametros) {
	global $db;

	$perfis = pegaPerfilGeral();
	
	if(is_array($parametros ['turid'])){

		foreach ($parametros ['turid'] as $turid){
			$dadosDiario = '';
			$sql = "select
						dia.hidid,
						dia.diaid,
						hid.stdid
					from
						projovemcampo.diario dia
					LEFT JOIN projovemcampo.historico_diario hid ON hid.hidid = dia.hidid
					where
						turid = '" . $turid . "'
					and perid = '" . $parametros ['perid'] . "' ";
		
			$dadosDiario = $db->pegaLinha ( $sql );
			
			// verifica historico anterior
			
			$anteriorHidid = $dadosDiario['hidid'];
			$anteriorHidid = $anteriorHidid ? $anteriorHidid : 'null';
			$anteriorstdid = $dadosDiario['stdid'];
			$anteriorstdid = $anteriorstdid ? $anteriorstdid : 'null';
			$Diario = $dadosDiario['diaid'];
			
			// verifica se já existe algum save para o mesmo histórico. caso sim, Ignore.
			
			if ($anteriorstdid != $parametros['status']) {

				$sql1 = "INSERT INTO projovemcampo.historico_diario ( stdid, diaid, anterior_hidid, usucpfquemfez, datahora ) values ({$parametros['status']}, $Diario, $anteriorHidid, '" . $_SESSION ['usucpf'] . "', clock_timestamp() ) RETURNING hidid";
			
				$pkHidid = $db->pegaUm ($sql1);
			
				// update id do historico na tabela de frequencia
				
				$sql2 .= "update projovemcampo.diario set hidid = $pkHidid where diaid = {$Diario} RETURNING true;";
				
			}
			
			$teste = $db->executar($sql2);
			
			if($status == ESTADO_DIARIO_PAGAMENTO_ENVIADO){
				$sqltestedocid = "SELECT
							estid,docid
						FROM
							projovemcampo.lancamentodiario lnd
						INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid
						WHERE
							(((lndhorasescola + lndhorascomunidade)*100)/(diatempoescola + diatempocomunidade))>= 75 
						AND dia.diaid = {$Diario} ";
				$testedocids = $db->carregar( $sqltestedocid );
				
				foreach ($testedocids as $testedocid){

					if($testedocid['docid']==''){
						$docid = '';
						$docid = wf_cadastrarDocumento( WORKFLOW_TIPODOCUMENTO_PAGAMENTO , 'Fluxo Pagamento Projovem Campo' );
						
						$sqlupdatedocid .="UPDATE projovemcampo.lancamentodiario
												SET
													docid = {$docid}
											WHERE
												estid = {$testedocid['estid']}
											AND diaid = {$Diario}
													";
					}
					
				}
				
				$db->executar ($sqlupdatedocid);
				
			}
			
		}
		
		$teste = $teste? 1 : 0;
		
		return $teste;
		
	}else{
		$dadosDiario = '';
		$sql = "select
				dia.hidid,
				dia.diaid,
				hid.stdid
			from
				projovemcampo.diario dia
			LEFT JOIN projovemcampo.historico_diario hid ON hid.hidid = dia.hidid
			where
				turid = '" . $parametros ['turid'] . "'
			and perid = '" . $parametros ['perid'] . "' ";

		$dadosDiario = $db->pegaLinha ( $sql );
		
		// verifica historico anterior
		
		$anteriorHidid = $dadosDiario['hidid'];
		$anteriorHidid = $anteriorHidid ? $anteriorHidid : 'null';
		$anteriorstdid = $dadosDiario['stdid'];
		$anteriorstdid = $anteriorstdid ? $anteriorstdid : 'null';
		$Diario = $dadosDiario['diaid'];
		
		// verifica se já existe algum save para o mesmo histórico. caso sim, Ignore.
		
		if ($anteriorstdid != $parametros['status']) {

			$sql1 = "INSERT INTO projovemcampo.historico_diario ( stdid, diaid, anterior_hidid, usucpfquemfez, datahora ) values ({$parametros['status']}, $Diario, $anteriorHidid, '" . $_SESSION ['usucpf'] . "', clock_timestamp() ) RETURNING hidid";
		
			$pkHidid = $db->pegaUm ( $sql1 );
		
			// update id do historico na tabela de frequencia
			
			$sql2 = "update projovemcampo.diario set hidid = $pkHidid where diaid = " . $Diario."RETURNING true";
			
			$teste = $db->pegaUm ( $sql2 );
			if($status == ESTADO_DIARIO_PAGAMENTO_ENVIADO){

				$sqltestedocid = "SELECT
									estid,docid
								FROM
									projovemcampo.lancamentodiario lnd
								INNER JOIN projovemcampo.diario dia ON dia.diaid = lnd.diaid
								WHERE
									(((lndhorasescola + lndhorascomunidade)*100)/(diatempoescola + diatempocomunidade))>= 75
								AND dia.diaid = {$Diario} ";
				$testedocids = $db->carregar( $sqltestedocid );
				
				foreach ($testedocids as $testedocid){
			
				if($testedocid['docid']==''){
					$docid = '';
					$docid = wf_cadastrarDocumento( WORKFLOW_TIPODOCUMENTO_PAGAMENTO , 'Fluxo Pagamento Projovem Campo' );
			
					$sqlupdatedocid .="UPDATE projovemcampo.lancamentodiario
										SET
											docid = {$docid}
										WHERE
											estid = {$testedocid['estid']}
										AND diaid = {$Diario}
										";
				}
							
			}
				
			$db->executar ($sqlupdatedocid);
				
		}
			
		$teste = $teste? 1 : 0;
			
		return $teste;
			
		}
	}
}
function buscarPeriodoDiario($dados) {
	global $db;

	$perId = '';
	if (isset ( $dados ['perid'] ))
		$perId = $dados ['perid'];
	$sqlRange = "
        			SELECT DISTINCT
						ordem
					FROM
						projovemcampo.diario dia
					INNER JOIN projovemcampo.rangeperiodo rap ON rap.rapid = dia.rapid
					WHERE
						turid = '{$dados['turid']}'";
// 	ver($sqlRange);
	$range = $db->pegaUm($sqlRange);
// 	ver($range);
	if($dados['gerarDiario']=='1'){
		if($range==''||empty($range)){

			$sql = "SELECT DISTINCT
						per.perid||'-'||rap.rapid as codigo,
						perdescricao ||' de '|| to_char(rap.datainicio,'DD/MM/YYYY') ||' a '||to_char(rap.datafim,'DD/MM/YYYY') as descricao
					FROM
						projovemcampo.periodo per
					INNER JOIN projovemcampo.rangeperiodo rap ON rap.perid = per.perid
					WHERE
						--datainicio <= '".date('Y-m-d')."'
					--AND	
						rap.perid = 1
					ORDER BY
						per.perid||'-'||rap.rapid";
			$dados = $db->carregar ( $sql );
			$dados ['bloq'] = $dados ['bloq'] ? $dados ['bloq'] : 'S';
		
			$db->monta_combo ( 'perid', $sql, $dados ['bloq'], 'Selecione', '', '', '', '', 'S', 'perid', '', $perId );
			
		}else{
//			Checa até qual diário a tuma ja enviou, permitindo assim a visualização do próximo.
			$sqlProximoPeriodo = "SELECT
									max(perid)
								FROM
									projovemcampo.diario dia
								INNER JOIN projovemcampo.historico_diario hid ON hid.hidid = dia.hidid
								WHERE
									dia.turid = '{$dados['turid']}'
								AND hid.stdid not in(1,12)";
			$ProximoPeriodo = $db->PegaUm($sqlProximoPeriodo);
			$ProximoPeriodo= $ProximoPeriodo ? $ProximoPeriodo : '0';
			
			$sql = "SELECT DISTINCT
						per.perid||'-'||rap.rapid as codigo,
						perdescricao ||' de '|| to_char(rap.datainicio,'DD/MM/YYYY') ||' a '||to_char(rap.datafim,'DD/MM/YYYY') as descricao
					FROM
						projovemcampo.periodo per
					INNER JOIN projovemcampo.rangeperiodo rap ON rap.perid = per.perid
					WHERE
						datainicio <= '".date('Y-m-d')."'
					AND	rap.ordem = {$range}
					AND per.perid <= {$ProximoPeriodo}+1";

			$dados ['bloq'] = $dados ['bloq'] ? $dados ['bloq'] : 'S';
			
			$db->monta_combo ( 'perid', $sql, $dados ['bloq'], 'Selecione', '', '', '', '', 'S', 'perid', '', $perId );
		}
	}elseif($dados['gerarDiario']=='2'){
		$sql = "SELECT DISTINCT
						per.perid||'-'||rap.rapid as codigo,
						perdescricao ||' de '|| to_char(rap.datainicio,'DD/MM/YYYY') ||' a '||to_char(rap.datafim,'DD/MM/YYYY') as descricao
					FROM
						projovemcampo.periodo per
					INNER JOIN projovemcampo.rangeperiodo rap ON rap.perid = per.perid
					INNER JOIN projovemcampo.diario dia ON per.perid = dia.perid
					WHERE
						--datainicio <= '".date('Y-m-d')."'
					--AND
						rap.perid = 1
					AND dia.turid = '{$dados['turid']}'
					ORDER BY
						per.perid||'-'||rap.rapid";
		$dadosperiodo = $db->carregar ( $sql );
		$dados ['bloq'] = $dados ['bloq'] ? $dados ['bloq'] : 'S';
		$db->monta_combo ( 'perid', $sql, $dados ['bloq'], 'Selecione', '', '', '', '', 'S', 'perid', '', $perId );
		
		if (!$dadosperiodo) {
			echo "<label style=\"color:red\">
				<b>
					Esta turma não possui período.
				</b>
				</label>";
		}
	}else{
		if($range!=''||!empty($range)){
			$sql = "SELECT DISTINCT 
						b.perid||'-'||rap.rapid as codigo, 
						b.perdescricao ||' de '|| to_char(rap.datainicio,'DD/MM/YYYY') ||' a '||to_char(rap.datafim,'DD/MM/YYYY') as descricao
					FROM projovemcampo.diario a
					INNER JOIN projovemcampo.periodo b ON a.perid = b.perid
					INNER JOIN projovemcampo.rangeperiodo rap ON rap.perid = b.perid
					WHERE 
						a.turid = '{$dados['turid']}'
					AND rap.ordem = '{$range}'
					ORDER BY 
						b.perid||'-'||rap.rapid ";
			$dadosperiodo = $db->carregar ( $sql );
			$dados ['bloq'] = $dados ['bloq'] ? $dados ['bloq'] : 'S';
		
			$db->monta_combo ( 'perid', $sql, $dados ['bloq'], 'Selecione', '', '', '', '', 'S', 'perid', '', $perId );
		}
		if (!$dadosperiodo) {
				echo "<label style=\"color:red\">
				<b>
				Esta turma não possui período.
				</b>
				</label>";
		}
	}
}
function montaCabecalhoDoDiarioFrequenciaMensal($parametros) {
	global $db;
/*(dia.diatempoescola +dia.diatempocomunidade)*/
	$sql = "SELECT
				dia.diatempoescola as tempoescola,
				dia.diatempocomunidade as tempocomunidade,
				dia.diaid,
				tur.entid AS id_escola,
				ent.entnome AS entidade,
				ende.endlog AS logradouro,
				ende.endnum AS numero,
				ende.endcom AS endereco_comercial,
				ende.endbai AS endbai,
				ende.endcep AS cep,
				ende.muncod AS codigo_municipal,
				ende.endlog || ende.endnum || ' - ' || ende.endbai ||' - '|| mun.mundescricao ||'/'|| ende.estuf as endereco_completo,
				mun.mundescricao AS municipio,
				ende.estuf AS uf,
				tur.turid AS turma_id,
				tur.turdescricao AS turma,
				per.perid,
				per.perdescricao AS periodo,
				rap.datainicio AS dt_inicio,
				rap.datafim AS dt_fim,
				coalesce((dia.diatempoescola +dia.diatempocomunidade),0) as soma_qtdauladada
				
			FROM 
				projovemcampo.diario dia
			INNER JOIN projovemcampo.periodo per ON per.perid = dia.perid
			INNER JOIN projovemcampo.rangeperiodo rap ON rap.rapid = dia.rapid
			INNER JOIN projovemcampo.turma tur ON tur.turid = dia.turid
			INNER JOIN entidade.entidade ent ON ent.entid = tur.entid
			INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
			INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
			WHERE
				dia.turid = {$parametros['turid']}
			AND	dia.perid = {$parametros['perid']}";
// 	ver($sql,d);
	$infoDiario = $db->pegaLinha ( $sql );
	return $infoDiario;
}
function listaEstudantesPorTurma($turid) {
	global $db;

	$sql = "SELECT 
				estid, 
				estnome, 
				eststatus,
				'E' as escola,
				'C' as comunidade
			FROM projovemcampo.estudante 
			WHERE 
				turid = ".$turid."
			ORDER BY 
				estnome ";
	// ver($sql);
	$retorno = $db->carregar ( $sql );
	return $retorno;
}
function listaPresencaPorAluno($param) {
	global $db;

	$diaid = ! empty ( $param ['diaid'] ) ? $param ['diaid'] : 0;
	$estid = ! empty ( $param ['estid'] ) ? $param ['estid'] : 0;

	$sql = "SELECT 
				lndhorasescola, 
				lndhorascomunidade
			FROM 
				projovemcampo.lancamentodiario
			WHERE 
				estid = {$estid}
			AND diaid = {$diaid}";

	$retorno = $db->pegaLinha ( $sql );

	return $retorno;
}
function salvarDiarioFrequenciaMensal($parametros) {
	global $db;
	
	$arrPerRap = spliti('-',$parametros['perid'],2);
	$parametros['perid'] = $arrPerRap[0];
	
	// Salva as aulas dadas em "diariofrequencia"
	$parEstudantes = $parametros ['qtdaulas'];
	$sql = "SELECT 
				diaid
			FROM projovemcampo.diario
			WHERE 
				turid = {$parametros['turid']}
			AND perid = {$parametros['perid']} ";
	
	$diaid = $db->pegaUm ( $sql );
	
	$parametros ['qtdaulasdadas']['C'] = $parametros ['qtdaulasdadas']['C']!=''?$parametros ['qtdaulasdadas']['C']:0;
	
	$parametros ['qtdaulasdadas']['E'] = $parametros ['qtdaulasdadas']['E']!=''?$parametros ['qtdaulasdadas']['E']:0;
	$sqlDadosDiario = "UPDATE projovemcampo.diario SET diatempocomunidade = {$parametros ['qtdaulasdadas']['C']}, diatempoescola = {$parametros ['qtdaulasdadas']['E']} WHERE turid = {$parametros['turid']} AND perid = {$parametros['perid']}";
	$db->executar ($sqlDadosDiario);
	
	$debugTotalUpdate = 0;
	$debugTotalInsert = 0;
	// Salva as presenças dos estudantes em "frequenciaestudante"
	foreach ( $parEstudantes as $chaveEstudante => $frequencia ) {

		$frequencia['C'] = $frequencia['C']!=''?$frequencia['C']:0;

		$frequencia['E'] =$frequencia['E']!=''?$frequencia['E']:0;
		$idDiario = $diaid;
		$sql = "SELECT 
					estid
				FROM projovemcampo.lancamentodiario
				WHERE 
					estid = {$chaveEstudante}
				AND diaid = {$idDiario} ";
			
		$temRegistro = $db->pegaLinha ( $sql );
		
		$sql = '';

		if($temRegistro['estid'] == '') {

			$sqlInsert .= "INSERT INTO projovemcampo.lancamentodiario(
				            diaid, estid, lndhorasescola, lndhorascomunidade
							)
				    VALUES ( $idDiario, {$chaveEstudante}, {$frequencia['E']}, {$frequencia['C']}
							);
							";

			$debugTotalInsert ++;
				
		} else {

			$sqlUpdate .= "UPDATE projovemcampo.lancamentodiario
					   SET lndhorasescola={$frequencia['E']}, lndhorascomunidade={$frequencia['C']} $adicionadocid
					 WHERE 
				 		estid = {$chaveEstudante}
                     AND diaid = {$idDiario};";

			$debugTotalUpdate ++;
		}
	// echo "debugTotalUpdate ". $debugTotalUpdate . " - debugTotalInsert: " . $debugTotalInsert;
	}

	if($sqlInsert!=''){
		$db->executar ( $sqlInsert );
	}
	if($sqlUpdate!=''){
		$db->executar ( $sqlUpdate );
	}
	$db->commit ();
}

//Funções encaminhar lista

function listaDeEncaminhamentoPerfilEquipeMEC($dados) {
	global $db;
	$retorno = '';

	$entid = ! empty ( $dados ['entid'] ) ? ' AND ent.entid  IN (' . $dados ['entid'] . ')' : '';
	$dadosEstuf = $dados ['estuf'];
	$estuf = ! empty ( $dados ['estuf'] ) ? " AND mun.estuf IN ('{$dadosEstuf}') " : "";
	$esfera = '';

	if ($dados ['esfera'] == 'M') {
		$esfera = " AND apc.apcesfera ='M'";
	} elseif ($dados ['esfera'] == 'E') {
		$esfera = " AND apc.apcesfera ='E'";
	}

	$estudantesaptos = ! empty ( $dados ['estudantesaptos'] ) ? ' AND (((lndhorasescola + lndhorascomunidade)*100)/(diatempoescola + diatempocomunidade))>= 75' : '';
	$estudantesinaptos = ! empty ( $dados ['estudantesinaptos'] ) ? ' AND (((lndhorasescola + lndhorascomunidade)*100)/(diatempoescola + diatempocomunidade))< 75' : '';
	$mundescricao = ! empty ( $dados ['mundescricao'] ) ? " AND mun.mundescricao ilike '%" . utf8_decode ( $dados ['mundescricao'] ) . "%'" : '';
	$naopagamento = ! empty ( $dados ['naopagamento'] ) ? ' AND hst.stdid <> ' . ESTADO_PAGAMENTO_PAGO : '';
	$simpagamento = ! empty ( $dados ['simpagamento'] ) ? ' AND hst.stdid =  ' . ESTADO_PAGAMENTO_PAGO : '';
	$esdid = ! empty ( $dados ['esdid'] ) ? " AND hst.stdid in(" . $dados ['esdid'] . ")" : '';

	$wherefiltro = $estudantesaptos . $estudantesinaptos . $naopagamento . $simpagamento . $entid . $polid . $mundescricao . $estuf . $esfera . $esdid;

	$wherefiltrotransferido = $estudantesaptos . $estudantesinaptos . $naopagamento . $simpagamento . $entid . $polid . $mundescricao . $estuf . $esfera . $esdid;

	if (! empty ( $_REQUEST [''] )) {
		$parametros ['estudantesaptos'] = $_REQUEST ['estudantesaptos'];
	}

// 	$dados ['from'] = sprintf ( " esd.esdid = %d OR esd.esdid = %d ", WF_ESTADO_DIARIO_APROVACAO, WF_ESTADO_DIARIO_VALIDACAO );
	$dados ['inner'] = "";
	$dados ['where'] = " AND est.eststatus = 'A'
						AND dia.perid      = " . $dados ['perid'] . $wherefiltro;
	
	// ver($dados['$wheretransferidos'],d);
	$sql = listaDeEncaminhamentoPerfilSQL ( $dados );

	// echo "PerfilEquipeMEC <pre>";print( $sql );exit;

	$retorno = $db->carregar ( $sql );
	$db->commit ();

	return $retorno;
}
function listaDeEncaminhamentoPerfilCoordenadorTurma($dados) {
	global $db;
	$retorno = '';

// 	$dados ['from'] = sprintf ( " esd.esdid = %d OR esd.esdid = %d ", WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_FECHADO );

	$dados ['inner'] = "--PerfilCoordenadorTurma
	                   LEFT JOIN projovemcampo.usuarioresponsabilidade rpu
								ON rpu.turid = tur.turid AND rpu.rpustatus = 'A' ";

	$dados ['where'] = "--PerfilCoordenadorTurma
	                   AND est.eststatus = 'A'
					   --AND apc.apcid     = " . $dados ['apcid'] . "
					   AND dia.perid     = " . $dados ['perid'] . "
					   AND rpu.usucpf    = '" . $dados ['usucpf'] . "'";

	$sql = listaDeEncaminhamentoPerfilSQL ( $dados );

	$retorno = $db->carregar ( $sql );
	$db->commit ();

	return $retorno;
}
function listaDeEncaminhamentoPerfilCoordenadorEstadual($dados) {
	global $db;
	$retorno = '';

// 	$dados ['from'] = sprintf ( " esd.esdid = %d OR esd.esdid = %d OR esd.esdid = %d", WF_ESTADO_DIARIO_VALIDACAO, WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ENCAMINHAR );

	$dados ['inner'] = "--PerfilCoordenadorEstadual
	                   	INNER JOIN projovemcampo.secretaria sec ON sec.secaid = apc.secaid";

	$dados ['where'] = "--PerfilCoordenadorEstadual
	                   AND est.eststatus = 'A'
					   --AND apc.apcid     = ".$_SESSION['projovemcampo']['apcid']."
					   AND dia.perid     = " . $dados ['perid'] . "
					   AND sec.secoordcpf     = '" . $dados ['usucpf'] . "'";
	
	$sql = listaDeEncaminhamentoPerfilSQL ( $dados );

	$retorno = $db->carregar ( $sql );
	$db->commit ();

	return $retorno;
}
function listaDeEncaminhamentoPerfilCoordenadorMunicipal($dados) {
	return listaDeEncaminhamentoPerfilCoordenadorEstadual ( $dados );
}
function listaDeEncaminhamentoPerfilDiretorDeEscola($dados) {
	global $db;
	$retorno = '';

// 	$dados ['from'] = sprintf ( " esd.esdid = %d OR esd.esdid = %d OR esd.esdid = %d ", WF_ESTADO_DIARIO_FECHADO, WF_ESTADO_DIARIO_ENCAMINHAR, WF_ESTADO_DIARIO_VALIDACAO );

// 	$dados ['inner'] = "--PerfilDiretorDeNucleo
//                         	INNER JOIN projovemcampo.usuarioresponsabilidade ur2 ON ur2.entid=ent.entid AND ur.rpustatus='A'  ";

	$dados ['where'] = "--PerfilDiretorDeNucleo
	                   AND est.eststatus = 'A'
					  -- AND apc.apcid     = " . $dados ['apcid'] . "
					   AND dia.perid     = " . $dados ['perid'] . "
					    AND ur.usucpf  = '" . $dados ['usucpf'] . "'";

	$sql = listaDeEncaminhamentoPerfilSQL ( $dados );

	$retorno = $db->carregar ( $sql );
	$db->commit ();

	return $retorno;
}
function listaDeEncaminhamentoPerfilSQL($paramentros) {
	global $db;
	$where = $paramentros ['where'];
	$inner = $paramentros ['inner'];
	$from = $paramentros ['from'];

				$perfis = pegaPerfilGeral ();

	$sql = "SELECT	DISTINCT
				--Por esfera estadual ou muncipal
				CASE WHEN apc.apcesfera = 'E'
					THEN 'Estadual'
					ELSE 'Municipal'
				END as esfera,
				--Informações do Estado
				mun.estuf as estuf, 
				COALESCE(mun.mundescricao,'Esfera Estadual') as mundescricao,
				--Informações do Escola
				ent.entid || usu.usucpf as cpfescola,
				ent.entid,
				'Escola '||' - DIRETOR: ' || usu.usunome as escola,
				--CASE WHEN nes.nuetipo = 'S' THEN 'SEDE : ' ELSE 'ANEXO : ' END as escola,
				--Informações da Turma
				tur.turid as turid,
				tur.turdescricao,
				--Informações do Estudante
				est.estid as matricula,
				est.estnome as estudante,
				dia.diaid,
				--Frequencia.	 
				CASE 
					WHEN (diatempoescola is not null AND diatempocomunidade is not null) AND (diatempoescola != 0 AND diatempocomunidade != 0)
					THEN (((lndhorasescola + lndhorascomunidade)*100)/(diatempoescola + diatempocomunidade)) 
					ELSE 0
					END as frequencia, 
				CASE 
					WHEN (est.estauxiliosareceber - est.estauxiliosrecebidos) > 0 
					THEN (est.estauxiliosareceber - est.estauxiliosrecebidos) ELSE 0 
				END as auxilios, 
				CASE 
					WHEN (diatempoescola is not null AND diatempocomunidade is not null) AND (diatempoescola != 0 AND diatempocomunidade != 0)
					THEN
						CASE
							WHEN ((((lndhorasescola + lndhorascomunidade)*100)/(diatempoescola + diatempocomunidade))>= 75 AND agbcod is not NULL) 
							THEN 'SIM' 
							ELSE 'NÃO'
						END
					ELSE
						'0'
				END as aptoreceber, 
				CASE WHEN LENGTH(est.estnis) = 0 or LENGTH(est.estnis) = 1
					THEN '-'
					ELSE COALESCE(est.estnis,'-')
				END AS nis,
				hst.stdid as estadodocumento,
				'regular' as tipo_aluno,
				agbcod as agbcod
			FROM
				projovemcampo.estudante est
			INNER JOIN projovemcampo.diario		dia	ON dia.turid	= est.turid
			INNER JOIN projovemcampo.historico_diario hst	ON hst.hidid	= dia.hidid
			INNER JOIN projovemcampo.lancamentodiario lnd	ON lnd.diaid	= dia.diaid AND est.estid = lnd.estid
			INNER JOIN projovemcampo.turma          tur	ON tur.turid   	= est.turid
			INNER JOIN entidade.entidade 	        ent	ON ent.entid   	= tur.entid
			INNER JOIN entidade.endereco		ende	ON ende.entid 	= ent.entid
			INNER JOIN territorios.municipio	mun	ON mun.muncod 	= ende.muncod	
			INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid  = tur.secaid
			INNER JOIN projovemcampo.usuarioresponsabilidade ur ON ur.entid=ent.entid AND ur.rpustatus='A' AND ur.pflcod = 1216
			INNER JOIN seguranca.usuario usu ON usu.usucpf = ur.usucpf
			LEFT JOIN  projovemcampo.agenciabancariaescola age ON age.entid = tur.entid
			$inner
			WHERE
				1=1
			$where	
			ORDER BY  esfera, estuf, mundescricao, turid, cpfescola, estudante				
			";
			
    									// echo "<pre>";print( $sql );//exit;
//     										ver($sql,d);
	return $sql;
}
// fim Funções encaminhar lista
function retornaNomeDaAgenciaCadastrada($paramentros) {
	global $db;
	$listaDeAgencias = array ();

	$sql = "SELECT DISTINCT
				case when abe.entid = ent.entid then
					" . $paramentros ['imgAgenciaVinculada'] . "
					" . $paramentros ['imgAgenciaComAcao'] . "
				else
					" . $paramentros ['imgAgenciaComAcao'] . "
				end as acao, 
				ent.entnome as escola
				, mun.estuf AS uf
				, mun.mundescricao AS municipio
				, ede.endlog ||',' ||ede.endnum|| '-' ||ede.endbai AS endereco
				, ede.endcep
				, case when abe.nabnomeagencia is null then
					abe.agbcod::varchar
				else
					abe.agbcod ||' / '|| abe.nabnomeagencia
				end as agencia
				, ede.muncod
				,case when abe.nabnomeagencia is null and abe.agbcod is not null 
					then
						1
					else
						0
				end as corrige_agencia
				, abe.abeid
				, abe.agbcod
			FROM projovemcampo.turma tur
			INNER JOIN entidade.entidade ent ON ent.entid   = tur.entid
			INNER JOIN entidade.endereco ede ON ede.entid   = ent.entid
			INNER JOIN territorios.municipio mun ON mun.muncod = ede.muncod
			LEFT JOIN projovemcampo.agenciabancariaescola abe ON abe.entid = tur.entid
			INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid  = tur.secaid
			WHERE
				ent.entstatus='A'
			AND tur.turstatus = 'A'
			AND 	apc.apcid ={$_SESSION['projovemcampo']['apcid']}
			order by
				municipio";
	$retorno = $db->carregar ( $sql );

	if ($retorno) {

		// Rotina para correção dos nomes das agências bancárias
		foreach ( $retorno as $chave => $valor ) {
			// Valida se registro precisa de correção e se tem código do muncipio
			if ($retorno [$chave] ['corrige_agencia'] == 1 && ! empty ( $retorno [$chave] ['muncod'] )) {
				// Se registro ainda não foi valido, então validá-lo
				if (! in_array ( $retorno [$chave] ['nabid'], $listaDeAgencias )) {
					// Chama Serviço de Agência
					$retornoWs = listaAgencias ( array (
							'muncod' => $retorno [$chave] ['muncod'],
							'uraiokm' => '500'
					) );
						
					// Insere registro na lista para não ser validado novamente
					$listaDeAgencias [] = $retorno [$chave] ['nabid'];
						
					// Lista de agências retornadas pelo Serviço WS
					foreach ( $retornoWs as $agencias ) {
						if ($retorno [$chave] ['agbcod'] == $agencias ['co_agencia']) {
							$sqlCorrecaoUpdate = "update projovemurbano.nucleoagenciabancaria
												  set nabnomeagencia = '" . $agencias ['no_agencia'] . "'
												  , nabdtatualizacao = current_timestamp
												  where nabid = " . $retorno [$chave] ['nabid'];
								
							$db->carregar ( $sqlCorrecaoUpdate );
						}
					}
				}
			}
		}

		$db->commit ();

		// Retorna uma nova consulta
		$retorno = $db->carregar ( $sql );

		// Remove os campos que não entrarão na tabela
		foreach ( $retorno as $chave => $valor ) {
			// Remove último registro do ARRAY, senão remover aparece no componente da tabela.
			array_pop ( $retorno [$chave] ); // muncod
			array_pop ( $retorno [$chave] ); // corrige_agencia
			array_pop ( $retorno [$chave] ); // nabid
			array_pop ( $retorno [$chave] ); // agbcod
		}
	} else {
		$retorno = array ();
	}

	return $retorno;
}
function listaAgencias($paramentros) {

	// Definindo os valores dos argumentos do webservice
	$sgUf = $_SESSION['projovemcampo']['estuf']; // Definindo como Distrito Federal
	$codIbge = $paramentros['muncod']; // 3514403
	$nuRaioKm = $paramentros['uraiokm']; // 10

	$cliente = new SoapClient ( "http://ws.mec.gov.br/AgenciasBb/wsdl" );
	$xmlDeRespostaDoServidor = $cliente->getMunicipio ( $codIbge, $nuRaioKm );

	$agencias = new SimpleXMLElement ( $xmlDeRespostaDoServidor );
	$retorno = array ();

	foreach ( $agencias->NODELIST as $agencia ){

		$coAgencia = $agencia->co_agencia . '-' . $agencia->nu_dv . '-' . utf8_encode ( $agencia->no_agencia );
		$arrAgencia = array (
				'co_agencia' => $agencia->co_agencia . '',
				'co_banco' => $agencia->co_banco . '',
				'dv' => $agencia->nu_dv . '',
				'agencia_dv' => $coAgencia,
				'no_agencia' => utf8_encode ( $agencia->no_agencia . '' )
		);

		$retorno [] = $arrAgencia;
	}

	return $retorno;
}
function contaEstudantesEscolas($turid) {
	global $db;

	// Adaptação para o perfil Diretor do Núcleo
	if (! $db->testa_superuser ()) {
		$perfis = pegaPerfilGeral ();
		if (in_array ( PFL_DIRETOR_ESCOLA, $perfis )) {
			$inner_nucleo = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='" . $_SESSION ['usucpf'] . "' and ur.entid=tur.entid AND rpustatus='A'";
		}
	}

	$sql = "SELECT DISTINCT
				count(estid) as qtd,
				tur.turqtdalunosprevistos
			FROM
			projovemcampo.turma tur
			INNER JOIN projovemcampo.estudante est ON est.turid = tur.turid
			INNER JOIN projovemcampo.adesaoprojovemcampo apc ON apc.secaid = tur.secaid
			{$inner_nucleo}
			WHERE
				tur.turstatus='A' 
			AND apc.apcstatus= 'A'
			AND est.apcid ='".$_SESSION['projovemcampo']['apcid']."'
        	AND tur.turid = {$turid} 
			GROUP BY
				tur.turqtdalunosprevistos";
			$qtd = $db->pegaLinha ( $sql );


	return $qtd ['qtd'] >= $qtd ['nucqtdestudantes'];
}
function inserirDadosLog($dados) {
	global $db;
	
	$sql = "INSERT INTO log_historico.logsgb_projovemcampo(
            lndid, logrequest, logresponse, logcpf, logcnpj, logservico,
            logdata, logerro, remid)
    		VALUES (".(($dados['lndid'])?"'".$dados['lndid']."'":"NULL").",
    				".(($dados['logrequest'])?"'".addslashes($dados['logrequest'])."'":"NULL").",
    				".(($dados['logresponse'])?"'".addslashes($dados['logresponse'])."'":"NULL").",
    				".(($dados['logcpf'])?"'".$dados['logcpf']."'":"NULL").",
    				".(($dados['logcnpj'])?"'".$dados['logcnpj']."'":"NULL").",
    				".(($dados['logservico'])?"'".$dados['logservico']."'":"NULL").",
    				NOW(),
    				".(($dados['logerro'])?$dados['logerro']:"NULL").",
    				".(($dados['remid'])?$dados['remid']:"NULL").");";
	
	$db->executar($sql);
	$db->commit();
}

function processarPagamentoBolsistaSGB($dados) {
	global $db;

	$sql = "SELECT parcela, docid, lndid FROM projovemcampo.lancamentodiario WHERE lndid='".$dados->id."'";
	$pagamentobolsista = $db->pegaLinha($sql);

	if($dados->situacao->codigo!='') {
		if($dados->situacao->codigo=='10001' ||
		$dados->situacao->codigo=='00023' ||
		$dados->situacao->codigo=='00025') {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_PAGAMENTO_ENVIAR, $cmddsc = '', array());
		} elseif($dados->situacao->codigo=='10002') {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_PAGAMENTO_REJEITAR, $cmddsc = 'Erro retornado pelo FNDE: '.$dados->situacao->codigo.' / '.$dados->situacao->descricao, array());
		} elseif($dados->situacao->codigo=='00058') {
				
			if($pagamentobolsista['parcela']) {

				$novaparcela = ($pagamentobolsista['parcela']+1);

			}else{
				
				$novaparcela = 1;

			}
			
			$sql = "UPDATE projovemcampo.historicopagamento SET parcela='".$novaparcela."' WHERE lndid='".$pagamentobolsista['lndid']."'";
			$db->executar($sql);
			
// 			$sql = "UPDATE projovemcampo.lancamentodiario SET remid=null, parcela='".$novaparcela."' WHERE lndid='".$pagamentobolsista['lndid']."'";
// 			$db->executar($sql);
			$db->commit();
				
		} else {
			echo wf_alterarEstado( $pagamentobolsista['docid'], AED_PAGAMENTO_REJEITAR, $cmddsc = 'Erro retornado pelo FNDE: '.$dados->situacao->codigo.' / '.$dados->situacao->descricao, array());
			$sql = "UPDATE projovemcampo.lancamentodiario SET remid=null WHERE lndid='".$pagamentobolsista['lndid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}

}

function analisaCodXML($xml,$cod) {
	if(strpos($xml, $cod.':')) {
		return 'FALSE';
	} else {
		return 'TRUE';
	}

}

function sincronizarDadosUsuarioSGB($dados) {
	global $db;
	
	set_time_limit( 0 );

	ini_set( 'soap.wsdl_cache_enabled', '0' );
	ini_set( 'soap.wsdl_cache_ttl', 0 );

	$opcoes = Array(
			'exceptions'	=> 0,
			'trace'			=> true,
			//'encoding'		=> 'UTF-8',
			'encoding'		=> 'ISO-8859-1',
			'cache_wsdl'    => WSDL_CACHE_NONE
	);
	 
	$soapClient = new SoapClient( WSDL_CAMINHO_CADASTRO, $opcoes );

	libxml_use_internal_errors( true );

	$sql = "SELECT
				est.estcpf,10 as nacid, est.estnome,est.estdatanascimento, est.estnomemae, est.estsexo, mun.muncod as co_municipio_ibge_nascimento, mun.estuf as sg_uf_nascimento,
				est.escid, lpad(abe.agbcod::char(4),4,'0') as iusagenciasugerida,m2.muncod as co_municipio_ibge, m2.estuf as sg_uf, est.estendlogradouro, est.estendcomplemento, est.estendnumero, est.estendcep, 
				est.estendbairro,est.estestufemissao, est.estnumrg, est.estdataemissaorg, estorgaoexpedidorg, est.estemail,usu.usuemail
			FROM
				projovemcampo.estudante est
			LEFT JOIN territorios.municipio mun on mun.muncod = est.estmuncodnasc
			INNER JOIN projovemcampo.turma tur ON tur.turid = est.turid
    		INNER JOIN projovemcampo.secretaria seca ON seca.secaid = tur.secaid
    		INNER JOIN projovemcampo.secretario seco ON seco.secoid = seca.secoid
    		INNER JOIN seguranca.usuario usu ON usu.usucpf = secocpf
			INNER JOIN projovemcampo.agenciabancariaescola abe ON abe.entid = tur.entid AND agbcod is not NULL 
			INNER JOIN entidade.endereco ende ON ende.entid = abe.entid
			LEFT JOIN territorios.municipio m2 ON m2.muncod = ende.muncod
    		WHERE est.estid in('".$dados['estid']."')";

	$dadosusuario = $db->pegaLinha($sql);
	if($dadosusuario) {

		// consultando se cpf existe no SGB
		$xmlRetorno = $soapClient->lerDadosBolsista(
				array('sistema' => SISTEMA_SGB,
						'login'   => USUARIO_SGB,
						'senha'   => SENHA_SGB,
						'nu_cpf'  => $dadosusuario['estcpf']
				)
		);
		
// 		if(!$dados['sincronizacao']) $lnscpf = $db->carregarColuna("SELECT lnscpf FROM projovemcampo.listanegrasgb");
// 		else $lnscpf = array();
// 		 ver(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['estcpf'],'logservico'=>'lerDadosBolsista'));
// 		if(!in_array($dadosusuario['estcpf'],$lnscpf)) {
			inserirDadosLog(array('logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['estcpf'],'logservico'=>'lerDadosBolsista'));
// 		} else {
// 			inserirDadosLog(array('logrequest'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logresponse'=>'Bolsista com problemas de characteres especiais no SGB. Adicionado a lista negra.','logcpf'=>$dadosusuario['estcpf'],'logservico'=>'lerDadosBolsista'));
// 			$existecpf = $dadosusuario['estcpf'];
// 		}

		preg_match("/<nu_cpf>(.*)<\\/nu_cpf>/si", $xmlRetorno, $match);
		 
		$xml = new SimpleXMLElement( $xmlRetorno );
		$existecpf = (string) $xml->nu_cpf;
		$existecpf = (string) $match[1];
		
		if($existecpf) $ac = 'A';
		else $ac = 'I';
		$dadosusuario['estendcomplemento'] = subistituiCaracteres($dadosusuario['estendcomplemento']);
// 		ver($dadosusuario['estemail'],$dadosusuario['usuemail'],d);
		// gravando dados do bolsista, se existir atualizar senão inserir
		$xmlRetorno_gravarDadosBolsista = $soapClient->gravarDadosBolsista(
				array('sistema'  => SISTEMA_SGB,
						'login'    => USUARIO_SGB,
						'senha'    => SENHA_SGB,
						'acao'     => $ac,
						'dt_envio' => date( 'Y-m-d' ),
						'pessoa'   => array('nu_cpf'                        => $dadosusuario['estcpf'],
								'no_pessoa'                     => removeAcentos( addslashes($dadosusuario['estnome']) ),
								'dt_nascimento' 				  => $dadosusuario['estdatanascimento'],
								'no_pai'        				  => '',
								'no_mae'        				  => removeAcentos( str_replace(array("'"),array(" "),$dadosusuario['estnomemae']) ),
								'sg_sexo'       				  => $dadosusuario['estsexo'],
								'co_municipio_ibge_nascimento'  => (($dadosusuario['co_municipio_ibge_nascimento'])?$dadosusuario['co_municipio_ibge_nascimento']:$dadosusuario['co_municipio_ibge']),
								'sg_uf_nascimento'              => (($dadosusuario['sg_uf_nascimento'])?$dadosusuario['sg_uf_nascimento']:$dadosusuario['sg_uf']),
								'co_estado_civil'               => $dadosusuario['escid'],
								'co_nacionalidade'              => $dadosusuario['nacid'],
								'co_situacao_pessoa'            => 1,
								'no_conjuge'                    => '',
								'ds_endereco_web'               => '',
								'co_agencia_sugerida'           => $dadosusuario['iusagenciasugerida'],
								'enderecos' 					  => array(array('co_municipio_ibge'       => $dadosusuario['co_municipio_ibge'],
								'sg_uf'                   => $dadosusuario['sg_uf'],
								'ds_endereco'             => removeAcentos( str_replace(array("'"),array(" "),str_replace(array("  "),array(""),$dadosusuario['estendlogradouro'])) ),
								'ds_endereco_complemento' => removeAcentos( str_replace(array("'"),array(" "),str_replace(array("  "),array(""),$dadosusuario['estendcomplemento']) ) ),
								'nu_endereco'             => removeAcentos( (($dadosusuario['estendnumero'])?$dadosusuario['estendnumero']:'0') ),
								'nu_cep'                  => $dadosusuario['estendcep'],
								'no_bairro'               => removeAcentos( str_replace(array("  "),array(""),addslashes($dadosusuario['estendbairro']))),
								'tp_endereco'             => 'R'
										)
						),
						'documentos' 				  	  => array(array('uf_documento'       => $dadosusuario['estestufemissao'],
						'co_tipo_documento'  => 2,
						'nu_documento'       => str_replace(array(" "," "),array("",""),str_replace(array("\'","'"),array(" "," "),$dadosusuario['estnumrg'])),
						'dt_expedicao'       => $dadosusuario['estdataemissaorg'],
						'no_orgao_expedidor' => removeAcentos(str_replace(array("'"),array(" "),$dadosusuario['estorgaoexpedidorg']))
						)
						),
						'emails'                        => array(array('ds_email' => $dadosusuario['estemail']!=''?$dadosusuario['estemail']:$dadosusuario['usuemail']?$dadosusuario['usuemail']:'inexistente@inexistente.com'
						)
						),
						'formacoes'                     => array( ),
						'experiencias'                  => array( ),
						'telefones'                     => array( ),
						'vinculacoes' 				  => array( )
						)
		)
		);
		
		$logerro_gravarDadosBolsista = analisaCodXML($xmlRetorno_gravarDadosBolsista,'10001');
		
		inserirDadosLog(array('logerro'=>$logerro_gravarDadosBolsista,'logrequest'=>$soapClient->__getLastRequest(),'logresponse'=>$soapClient->__getLastResponse(),'logcpf'=>$dadosusuario['estcpf'],'logservico'=>'gravarDadosBolsista'));
		
		$sql = "UPDATE projovemcampo.estudante SET cadastradosgb=".(($logerro_gravarDadosBolsista=='TRUE')?'FALSE':'TRUE')." WHERE estid='".$dados['estid']."'";
		$db->executar($sql);
		$db->commit();

	}

}