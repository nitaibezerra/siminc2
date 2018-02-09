<?php

function recuperaMunicipioEstado()
{
	global $db;

	if($_SESSION['snf']['muncod']){
		$stCampo = "mun.mundescricao || ' - ' || est.estuf as descricao";
		$stInner = "LEFT JOIN territorios.municipio mun ON mun.estuf = est.estuf";
		$stWhere = "WHERE mun.muncod = '{$_SESSION['snf']['muncod']}'";
	}else{
		$stCampo = "est.estdescricao || ' - ' || est.estuf as descricao";
		$stWhere = "WHERE est.estuf = '{$_SESSION['snf']['estuf']}'";
	}

	$sql = "SELECT
				{$stCampo}
			FROM territorios.estado est
			{$stInner}
			{$stWhere}
			LIMIT 1";

	return $db->pegaUm($sql);
}


function setarInuIdPorEstuf($dados) {
	global $db;
	
	$sql = "select * from par.instrumentounidade iu 
			left join territorios.estado es on es.estuf = iu.estuf  
			where iu.itrid=1 and iu.estuf='".$dados['estuf']."'";
	$registro = $db->pegaLinha($sql);
	
	$_SESSION['snf']['itrid'] = $registro['itrid'];
	$_SESSION['snf']['inuid'] = $registro['inuid'];
	$_SESSION['snf']['estcod'] = $registro['estcod'];
	$_SESSION['snf']['estuf'] = $dados['estuf'];
	
	if($dados['goto']) {
		echo "<script>window.location='snf.php?modulo=principal/".$dados['goto']."&acao=A';</script>";
	}
	
}


function montaMenuAtendimentoForum() {
	
	$menu = array(0 => array("id" => 1, "descricao" => "Diagnóstico do PAR",   					"link" => "/snf/snf.php?modulo=principal/diagnosticoPar&acao=A"),
				  1 => array("id" => 2, "descricao" => "IDE", 			   						"link" => "/snf/snf.php?modulo=principal/ide&acao=A"),
				  2 => array("id" => 3, "descricao" => "Formação Inicial",    					"link" => "/snf/snf.php?modulo=principal/formacaoInicial&acao=A"),
				  3 => array("id" => 4, "descricao" => "Formação Continuada",  					"link" => "/snf/snf.php?modulo=principal/programacaoAtendimento&acao=A"),
				  4 => array("id" => 5, "descricao" => "Anexos", 								"link" => "/snf/snf.php?modulo=principal/anexoforum&acao=A"),
				  5 => array("id" => 6, "descricao" => "Síntese",    							"link" => "/snf/snf.php?modulo=principal/sintese&acao=A")
			  	  );
			  	  
	return $menu;
	
}


function pegaInsid(){
	
	global $db;
	
	$sql = "SELECT insid 
			FROM snf.usuarioresponsabilidade
			WHERE usucpf = '".$_SESSION['usucpf']."'";
	
	return $db->pegaUm($sql);
}

function checkPerfil( $pflcods )
{
	global $db;
	//if ($db->testa_superuser()) {
	//return true;
	//}else{
	if ( is_array( $pflcods ) ){
		$pflcods = array_map( "intval", $pflcods );
		$pflcods = array_unique( $pflcods );
	}
	else{
		$pflcods = array( (integer) $pflcods );
	}
	if ( count( $pflcods ) == 0 ){
		return false;
	}
	$sql = "SELECT COUNT(*)
			FROM seguranca.perfilusuario
			WHERE usucpf = '" . $_SESSION['usucpf'] . "'
			AND pflcod in ( " . implode( ",", $pflcods ) . " ) ";
	return $db->pegaUm( $sql ) > 0;
	//}
}

function naoExisteInstituicao()
{
	global $db;

	$texto = "Favor informar a Instituição da Rede Nacional de Formação na aba 'Dados da Instituição'.";
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr class="center SubtituloTabela" >
			<td colspan="2">
				<p class="red bold" ><?php echo $texto ?></p>
			</td>
		</tr>
	</table>
	<?php

}

function naoExisteMatenedora()
{
	$texto = "Favor informar a Mantenedora da Instituição da Rede Nacional de Formação na aba 'Dados da Mantenedora'.";
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr class="center SubtituloTabela" >
			<td colspan="2">
				<p class="red bold" ><?php echo $texto ?></p>
			</td>
		</tr>
	</table>
	<?php
}

function naoExisteDirigente()
{
	$texto = "O seu CPF não encontra-se vinculado a uma Instituição de Ensino. Favor entrar em contato com o gestor do sistema.";
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr class="center SubtituloTabela" >
			<td colspan="2">
				<p class="red bold" ><?php echo $texto ?></p>
			</td>
		</tr>
	</table>
	<?php
}

function instituicaoNaoExistente()
{
	$texto = "Instituição de Ensino não encontrada. Favor entrar em contato com o gestor do sistema.";
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr class="center SubtituloTabela" >
			<td colspan="2">
				<p class="red bold" ><?php echo $texto ?></p>
			</td>
		</tr>
	</table>
	<?php
}


function filtraCidade()
{
	global $db;

	$estuf = $_POST['estuf'];
	$obrigatorio = $_POST['obrigatorio'] ? $_POST['obrigatorio'] : "S";
	$name = $_POST['name'] ? $_POST['name'] : "muncod";
	$sql = "select
   				muncod as codigo,
   				mundescricao as descricao
   			from
   				territorios.municipio
   			where
   				estuf = '$estuf'
   			order by
   				mundescricao";
	if(!$estuf){
		$db->monta_combo($name,$sql,"N","Selecione...","","","","200",$obrigatorio, 'muncod1');
	}else{
   		$db->monta_combo($name,$sql,"S","Selecione...","","","","200",$obrigatorio, 'muncod1');
	}
   	exit;
}

function verificaCPFRF()
{
	$cpf = $_POST['cpf'];
	if(!validaCPFRF($cpf)){
		echo "CPF inválido.";
		exit;
	}

	require_once APPRAIZ . 'www/includes/webservice/cpf.php';
	ob_clean();
	$objPessoaFisica = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
	$cpf = str_replace(array('/', '.', '-'), '', $cpf);
	$xml = $objPessoaFisica->solicitarDadosPessoaFisicaPorCpf($cpf);
	$obj = (object) simplexml_load_string($xml);
	if (!$obj->PESSOA) {
		echo "CPF inexistente na base da Receita Federal.";
		exit;
	}else{
		echo "Nome:".$obj->PESSOA->no_pessoa_rf;
		exit;
	}

}

function validaCPFRF($cpf)
{

	// Verifiva se o número digitado contém todos os digitos
    $cpf = str_pad(ereg_replace('[^0-9]', '', $cpf), 11, '0', STR_PAD_LEFT);

	// Verifica se nenhuma das sequências abaixo foi digitada, caso seja, retorna falso
    if (strlen($cpf) != 11 || $cpf == '00000000000' || $cpf == '11111111111' || $cpf == '22222222222' || $cpf == '33333333333' || $cpf == '44444444444' || $cpf == '55555555555' || $cpf == '66666666666' || $cpf == '77777777777' || $cpf == '88888888888' || $cpf == '99999999999')
	{
		return false;
    }
	else
	{   // Calcula os números para verificar se o CPF é verdadeiro
        for ($t = 9; $t < 11; $t++) {
            for ($d = 0, $c = 0; $c < $t; $c++) {
                $d += $cpf{$c} * (($t + 1) - $c);
            }

            $d = ((10 * $d) % 11) % 10;

            if ($cpf{$c} != $d) {
                return false;
            }
        }

        return true;
    }
}

function htmlAddNovoMembro()
{
	global $db;

	$numero = $_POST['numero'];

	?>
	<tr class="SubtituloTabela" >
		<td colspan="2">
			Membro <?php echo  $numero - 1 ?>
			<img src="../imagens/excluir.gif" class="img_middle link" style="background-color:#FFFFFF" onclick="excluiMembroComite(this)"   />
		</td>
	</tr>
	<tr>
		<td class="SubtituloDireita" width="25%">
			CPF:
		</td>
		<td>
			<?php echo campo_texto("memcpf[]","S","S","",18,14,"###.###.###-##","","","",""," id='memcpf_$numero' onchange='verificaCPFRF(this)' ") ?>
		</td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >
			Nome:
		</td>
		<td>
			<?php echo campo_texto("memnome[]","S","N","",60,255,"","","","",""," id='memnome_$numero' ") ?>
		</td>
	</tr>
	<tr>
		<td class="SubtituloDireita" >
			Papel:
		</td>
		<td>
			<?php $sql = "select
							papid as codigo,
							papdescricao as descricao
						from
							snf.papelcomite
						where
							papstatus = 'A'
						order by
							papdescricao" ?>
			<?php $db->monta_combo("papid[]",$sql,"S","Selecione...","mostrarOutros(this)","","","200","S") ?>
		</td>
	</tr>
        <tr class="tr_outros" style="display: none;">
                                <td class="SubtituloDireita" >
                                        Outros:
                                </td>
                                <td>
                                    <?php echo campo_texto("memobs[]","S","S","",30,50,"","","","","","") ?>
                                </td>
                        </tr>
	<?php
	exit;
}

function salvarComite()
{
	global $db;

	extract($_POST);

	//1º Insere o arquivo do Ato de Nomeação do Dirigente
	/*if($_FILES['arquivo_dirigente']['size']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$campos	= array("anedatahora" => "now()","anestatus" => "'A'","anetipo" => "1");
		$file   = new FilesSimec("anexo", $campos, "snf");
		$file->setUpload("Documento do ato de nomeação do dirigente.","arquivo_dirigente");
		$arqid = $file->getIdArquivo();
		$sql = "select aneid from snf.anexo where arqid = $arqid";
		$aneid = $db->pegaUm($sql);
	}*/

	//2º - Atualiza o cargo e o anexo (se existir) do dirigente
	if($aneid){
		$sql = "update snf.dirigentemaximo set carid = $carid, aneid = $aneid where dirid = $dirid";
	}else{
		$sql = "update snf.dirigentemaximo set carid = $carid where dirid = $dirid";
	}
	$db->executar($sql);

	//3.1º Insere o arquivo de criação do comitê.
	if($_FILES['arquivo_comite']['size']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$campos	= array("anedatahora" => "now()","anestatus" => "'A'","anetipo" => "2");
		$file   = new FilesSimec("anexo", $campos, "snf");
		$file->setUpload("Documento de formalização da criação do comitê","arquivo_comite");
		$arqid = $file->getIdArquivo();
		$sql = "select aneid from snf.anexo where arqid = $arqid";
		$aneidComite = $db->pegaUm($sql);
	}
        
	//3.2º Insere o arquivo de criação do comitê.
	if($_FILES['arquivo_comite2']['size']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$campos	= array("anedatahora" => "now()","anestatus" => "'A'","anetipo" => "2");
		$file   = new FilesSimec("anexo", $campos, "snf");
		$file->setUpload("Documento de Nomeação do Coordenador Institucional","arquivo_comite2");
		$arqid = $file->getIdArquivo();
		$sql = "select aneid from snf.anexo where arqid = $arqid";
		$aneidComite2 = $db->pegaUm($sql);
	}
        
	//4º Verifica se exite comitê.
	$sql = "select
				comid
			from
				snf.comite
			where
				insid = $insid
			and
				comstatus = 'A'";
	$comid = $db->pegaUm($sql);

	//5º Insere ou atualiza o comitê, com o id da instituição (insid) e o id do anexo da criação do comitê (aneid -> $aneidComite)
	if(!$comid){
		$aneidComite = $aneidComite ? $aneidComite : "null";
		$aneidComite2 = $aneidComite2 ? $aneidComite2 : "null";
		$sql = "insert into snf.comite (insid,comstatus,aneid , aneid2) values ($insid,'A',$aneidComite, $aneidComite2) returning comid";
		$comid = $db->pegaUm($sql);
	}else{
		if($aneidComite){
			$sql = "update snf.comite set aneid = $aneidComite where comid = $comid";
			$db->executar($sql);
		}
		if($aneidComite2){
			$sql = "update snf.comite set aneid2 = $aneidComite2 where comid = $comid";
			$db->executar($sql);
		}
	}

	//6º Deleta todos os membros do comitê, para inserir os corretos novamente
	$sql = "delete from snf.membrocomite where comid = $comid";
	$db->executar($sql);

	//7º verifica se existem mebros para serem inseridos ou atualizados
	if($_REQUEST['memcpf']){
		$n = 0;
		foreach($_REQUEST['memcpf'] as $memcpf){
			$memcpf = str_replace(array("-","."),array("",""),$memcpf);
                        if($_REQUEST['papid'][$n] == 8){
                            $memobs = "'{$_REQUEST['memobs'][$n-1]}'";
//                            if($memobs){
//                                ver($_REQUEST['memnome'][$n], $_REQUEST['papid'][$n], $memobs, $n, $_REQUEST['memobs'], $_REQUEST['memnome'],d);
//                            }
                        } else {
                            $memobs = 'NULL';
                        }
                        
                        
			//Apenas o 1º mebro possui campos de telefone, endereço, e-mail, estado e município
			if($n == 0){
				$sqlI = "insert into
							snf.membrocomite
						(comid,papid,memnome,memcpf,memendereco,memtelefone,mememail,memstatus,estuf,muncod)
							values
						($comid,{$_REQUEST['papid'][$n]},'{$_REQUEST['memnome'][$n]}','$memcpf','{$_REQUEST['memendereco']}','{$_REQUEST['memtelefone']}','{$_REQUEST['mememail']}','A','{$_REQUEST['estuf']}','{$_REQUEST['muncod']}');";
			}else{
				$sqlI.= "insert into
							snf.membrocomite
						(comid,papid,memnome,memcpf,memstatus, memobs)
							values
						($comid,{$_REQUEST['papid'][$n]},'{$_REQUEST['memnome'][$n]}','$memcpf','A', {$memobs});";
			}
			$n++;
		}
		//8º Insere os membros do comitê
		$db->executar($sqlI);
	}

	//9º Atualiza os dados de endereço da instituição
	$endcep = str_replace(array("-","."),array("",""),$endcep);
	if($endid){
		$sqlE = "update
					snf.endereco
				set
					muncod = '$muncod_ende',
					estuf = '$estuf_ende',
					endlog = '$endlog',
					endcom = '$endcom',
					endbai = '$endbai',
					endnum = '$endnum',
					endcep = '$endcep'
				where
					insid = $insid
				and
					endid = $endid";
	}else{
		$sqlE = "insert into
					snf.endereco
				(muncod,estuf,endcep,endlog,endcom,endbai,endnum,insid, endstatus)
					values
				('$muncod_ende','$estuf_ende','$endcep','$endlog','$endcom','$endbai','$endnum',$insid, 'A')";
	}
	$db->executar($sqlE);

	//10º Atualiza os dados da Instituição
	$inscnpj = str_replace(array("-",".","/"),array("","",""),$inscnpj);
	$sqlU = "update
				snf.instituicaoensino
			set
				insnome = '$insnome',
				inssigla = '$inssigla',
				instelefone = '$instelefone',
				insfax = '$insfax',
				insemail = '$insemail',
				inscnpj = '$inscnpj'
			where
				insid = $insid";
	$db->executar($sqlU);

	//11º Comita todas as alterações
	$db->commit();

	//12º Se existir valor na varíavel $continuar, deve ir pra tela do termo, caso contrário, permanece na mesma tela
	$url_destino = $continuar ? "gerarTermoAdesao" : "aderirTermoAdesao";
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='snf.php?modulo=principal/$url_destino&acao=A';
		  </script>";
	exit;

}

function mascaraglobalTermoAdesao($value, $mask) {
	$casasdec = explode(",", $mask);
	// Se possui casas decimais
	if($casasdec[1])
		$value = sprintf("%01.".strlen($casasdec[1])."f", $value);

	$value = str_replace(array("."),array(""),$value);
	if(strlen($mask)>0) {
		$masklen = -1;
		$valuelen = -1;
		while($masklen>=-strlen($mask)) {
			if(-strlen($value)<=$valuelen) {
				if(substr($mask,$masklen,1) == "#") {
						$valueformatado = trim(substr($value,$valuelen,1)).$valueformatado;
						$valuelen--;
				} else {
					if(trim(substr($value,$valuelen,1)) != "") {
						$valueformatado = trim(substr($mask,$masklen,1)).$valueformatado;
					}
				}
			}
			$masklen--;
		}
	}
	return $valueformatado;
}

function verificaRegrasGerarTermo()
{
	global $db;

	//Regra - É obrigatório a informação do dirigente máximo da instituição
	$sql = "	select
					dir.dirid
				from
					snf.dirigentemaximo dir
				inner join
					snf.instituicaoensino ins ON ins.insid = dir.insid
				where
					ins.insid = {$_SESSION['snf']['insid']}
				and
					dirstatus = 'A'";
	$arrDirigente = $db->pegaLinha($sql);
	if(!$arrDirigente){
		//return "É necessário informar o Dirigente Máximo e incluir o documento de nomeação do Dirigente.";
		return "É necessário informar o Dirigente Máximo da Instituição.";
	}


	
	
	//Regra - É obrigatório que o comitê tenha um membro como Coordenador institucional, um com Coordenação das Licenciaturas ou equivalente e um com o	Pró-Reitoria de Graduação ou equivalente
	$sql = "select
					*
				from
					snf.comite com
				left join
					snf.anexo anx ON anx.aneid = com.aneid and anestatus = 'A'
				left join
					public.arquivo arq ON anx.arqid = arq.arqid
				left join
					snf.membrocomite mem ON com.comid = mem.comid and memstatus = 'A'
				where
					com.insid = {$_SESSION['snf']['insid']}
				and
					comstatus = 'A'
				and
					anestatus = 'A'";
	$arrDados = $db->carregar($sql);

	if($arrDados){
		foreach($arrDados as $dado){
			if($dado['papid'] == PAPEL_COORD_INSTITUCIONAL && !$arrCood['arqid']){
				$coordenar_institucional = true;
			}
			if($dado['papid'] == PAPEL_COORD_LICENCIATURA){
				$coordenar_licenciatura = true;
			}
			if($dado['papid'] == PAPEL_PRO_REITORIA_GRADUACAO){
				$coordenar_pro_reitoria_graduacao = true;
			}
		}
	}//else{
		//return "É necessário informar o Coordenador Institucional do Comitê Gestor e o Documento de formalização da criação do comitê.";
		//$msn .= "Favor indicar o(a) Coordenador(a) Institucional e preencher os campos obrigatórios. ";
	//}

	if(!$coordenar_institucional){
		//return "É necessário informar o Coordenador Institucional do Comitê Gestor e o Documento de formalização da criação do comitê.";
		$msn .= "Favor indicar o(a) Coordenador(a) Institucional e preencher os campos obrigatórios. ";
	}

	if(!$coordenar_licenciatura){
		//return "É necessário informar pelo menos um membro para a Coordenação das Licencitaruas ou equivalente.";
		$msn .= "É necessário indicar pelo menos um representante das Licenciaturas ou função equivalente. ";
	}
	if(!$coordenar_pro_reitoria_graduacao){
		//return "É necessário informar pelo menos um membro para a Pró-Reitoria de Graduação ou equivalente.";
		$msn .= "É necessário indicar pelo menos um representante da Pró-Reitoria de Graduação ou função equivalente. ";
	}

	//return "ok";
	return $msn;

}

function gerarTermoPDF()
{
	global $db;

	include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
	$html = geraTermoInstituicaoHtml($_SESSION['snf']['insid']);
	$html = utf8_encode($html);
	$http = new RequestHttp();

	//º O termo deve ser gerado e gravado na tabela snf.termoadesao, a partir de então, não poderá ser gerado novamente.
	$sql = "select aneid from snf.termoadesao where insid = {$_SESSION['snf']['insid']}";
	$aneid= $db->pegaUm($sql);
	if(!$aneid){
		// Envia um e-mail com a descrião da nova instituição que aderiu ao SNF para os usuários com perfil ADMINISTRADOR
		enviaEmailAdministradores($_SESSION['snf']['insid']);
		salvaTermoGerado($http->toPdf($html));
	}
	$http->toPdfDownload($html,"termo_adesao_".date("d_m_Y"));
}

function geraTermoInstituicaoHtml($insid)
{
	global $db;

	$sql = "select
				(case when inssigla is not null
					then inssigla || ' - ' || insnome
					else insnome
				end) as insnome,
				dirnome,
				dircpf,
				ins.inscnpj as entnumcpfcnpj,
				mun.mundescricao,
				mun.estuf
			from
				snf.instituicaoensino ins
			inner join
				snf.dirigentemaximo dir ON dir.insid = ins.insid
			left join
				entidade.endereco ende2 on ende2.entid = ins.entid --and ende2.endstatus = 'A'
			left join
				territorios.municipio mun on mun.muncod = ende2.muncod
			where
				ins.insid = '{$_SESSION['snf']['insid']}'
			and
				insstatus = 'A'
			and
				dirstatus = 'A'";

	$arrDados = $db->pegaLinha($sql);

	$html = '<center><img src="http://simec.mec.gov.br/imagens/brasao.gif" style="width:100px;height:100px"   /></center>
	<center><h3>MINISTÉRIO DA EDUCAÇÃO</h3></center>
	<center><h3>TERMO DE ADESÃO À REDE NACIONAL DE FORMAÇÃO CONTINUADA DOS PROFISSIONAIS DO MAGISTÉRIO DA EDUCAÇÃO BÁSICA PÚBLICA</h3></center>
	<p style="text-align:justify;font-size:14px" >O(A) '.$arrDados['insnome'].', inscrita no CNPJ sob o nº '.mascaraglobalTermoAdesao($arrDados['entnumcpfcnpj'],"##.###.###/####-##").', neste ato representado(a) por seu(sua) dirigente máximo(a), '.$arrDados['dirnome'].' - CPF: '.mascaraglobalTermoAdesao($arrDados['dircpf'],"###.###.###-##").', resolve formalizar sua adesão à Rede Nacional de Formação Continuada dos Profissionais do Magistério da Educação Básica Pública.</p>
	<h4>DO OBJETIVO</h4>
	<p style="text-align:justify;font-size:14px" >CLÁUSULA PRIMEIRA – Participar como Instituição de Ensino Superior formadora da oferta de cursos e programas no âmbito da Rede Nacional de Formação Continuada dos Profissionais do Magistério da Educação Básica Pública, nos termos da Portaria MEC n° 1.328, de 23 de setembro de 2011, publicada na página 14 da seção 01 do Diário Oficial da União no dia 26 de setembro de 2011 e das demais normas que venham a substituir ou complementar a legislação vigente, habilitando-se ao recebimento de recursos do MEC destinados a fomentar as ações da Rede e em atendimento às demandas de formação continuada formuladas nos planos estratégicos de que tratam os artigos 4º, 5º, e 6º do Decreto n° 6.755, de 29 de janeiro de 2009.</p>
	<h4>DA ADESÃO</h4>
	<p style="text-align:justify;font-size:14px" >CLÁUSULA SEGUNDA – Esta adesão, solicitada de forma eletronicamente pelo titular da Instituição de Ensino Superior ou Instituto Federal de Educação, Ciência e Tecnologia, junto com o ato de nomeação do signatário e do ato constitutivo do Comitê Gestor Institucional de Formação de Profissionais do Magistério da Educação Básica, tem eficácia após validação pelo Ministério da Educação.</p>
	<p style="text-align:justify;font-size:14px" >Parágrafo único: O apoio financeiro concedido à Instituição de Ensino Superior ou Instituto Federal de Educação, Ciência e Tecnologia será realizado a partir do próximo exercício fiscal, desde que a adesão ocorra até 31 de maio, ou somente a partir do exercício seguinte, se a adesão for posterior.</p>
	<h4>DA PARTICIPAÇÃO</h4>
	<p style="text-align:justify;font-size:14px" >CLÁUSULA TERCEIRA – A adesão abrange Instituições de Educação Superior (IES), públicas e comunitárias sem fins lucrativos, e Institutos Federais de Educação, Ciência e Tecnologia (IF) habilitados a ofertar cursos ou programas de formação continuada aos profissionais do magistério da educação básica de forma articulada com os sistemas de ensino e com os Fóruns Estaduais Permanentes de Apoio à Formação Docente.</p>
	<p style="text-align:justify;font-size:14px" >Parágrafo único: Os cursos e programas de formação continuada, após homologação no Comitê Gestor Institucional de Formação de Profissionais do Magistério da Educação Básica, deverão ser submetidos pelas Instituições de Ensino Superior e Institutos Federais de Educação, Ciência e Tecnologia, periodicamente, nos termos e prazos definidos pelos Fóruns Estaduais Permanentes de Apoio à Formação Docente, para posterior aprovação do fomento pelo MEC.</p>
	<h4>DA VIGÊNCIA</h4>
	<p style="text-align:justify;font-size:14px" >CLÁUSULA QUARTA – Uma vez formalizada a adesão à Rede Nacional de Formação Continuada dos Profissionais do Magistério da Educação Básica Pública, sua vigência é válida por tempo indeterminado, ou até que seja solicitado o seu cancelamento pela Instituição de Ensino Superior ou Instituto Federal de Educação, Ciência e Tecnologia, a qualquer tempo, mediante ofício assinado por seu titular ao Comitê Gestor da Política Nacional de Formação Inicial e Continuada de Profissionais da Educação Básica, implicando a interrupção definitiva do apoio financeiro aos cursos e programas fomentados pelo MEC.</p>
	<h4>DA ALTERAÇÃO OU DESISTÊNCIA</h4>
	<p style="text-align:justify;font-size:14px" >CLÁUSULA QUINTA – Fica a Instituição de Ensino Superior ou Instituto Federal de Educação, Ciência e Tecnologia obrigado a solicitar a alteração do Plano de Trabalho para fomento de cursos e programas no âmbito da Rede Nacional de Formação Continuada dos Profissionais do Magistério da Educação Básica, sempre que caracterizada necessidade de alteração ou desistência de oferta, mediante envio de ofício do titular ao Comitê Gestor da Política Nacional de Formação Inicial e Continuada de Profissionais da Educação Básica, para interrupção do apoio financeiro, com duração sujeita aos mesmos prazos descritos no parágrafo segundo da Cláusula Segunda.</p>
	<h4>DA PUBLICIDADE</h4>
	<p style="text-align:justify;font-size:14px" >CLÁUSULA SEXTA – As opções por adesão, seu cancelamento, alteração ou desistência de oferta serão divulgadas em listas publicadas no Portal do Ministério da Educação na internet.</p>
	<p style="text-align:justify;font-size:14px" >E, por estar de acordo com todas as condições e cláusulas deste Termo de Adesão, firmo o presente instrumento.</p>
	<center><p style="font-size:14px" >'.date("d/m/Y").' - '.$arrDados['mundescricao'].'/'.$arrDados['estuf'].'</p></center>
	<center><p style="font-size:14px" >'.$arrDados['dirnome'].'<br/>
	'.mascaraglobalTermoAdesao($arrDados['dircpf'],"###.###.###-##").'</p></center>';

	return $html;
}

function pegaDadosMantenedora()
{
	global $db;

	//Recupera os dados de acordo com o CPF do usuário, que deve ser dirigente da instituição
	$sql = "select
				man.manid,
				man.co_mantenedora,
				emec.nu_cnpj as cnpj,
				emec.no_razao_social as nome,
				emec.sg_mantenedora as sigla,
				natureza_juridica as natureza_juridica,
				'N/A' as representante_legal,
				'N/A' as cpf_representante
			from
				snf.mantenedora man
			inner join
				emec.mantenedora emec ON man.co_mantenedora = emec.co_mantenedora
			inner join
				emec.ies ies ON ies.co_mantenedora = man.co_mantenedora
			inner join
				emec.naturezajuridica nat ON nat.co_natureza_juridica_gn = emec.co_natureza_juridica_gn
			inner join
				emec.dirigente dir ON dir.co_ies = ies.co_ies
			where
				dir.nu_cpf = '{$_SESSION['usucpf']}'
			and
				man.manstatus = 'A'";

	$arrDados = $db->pegaLinha($sql);

	if(!$arrDados){
		$sql = "select
				man.co_mantenedora,
				nu_cnpj as cnpj,
				no_razao_social as nome,
				sg_mantenedora as sigla,
				natureza_juridica as natureza_juridica,
				'N/A' as representante_legal,
				'N/A' as cpf_representante
			from
				emec.mantenedora man
			inner join
				emec.ies ies ON ies.co_mantenedora = man.co_mantenedora
			inner join
				emec.naturezajuridica nat ON nat.co_natureza_juridica_gn = man.co_natureza_juridica_gn
			inner join
				emec.dirigente dir ON dir.co_ies = ies.co_ies
			where
				dir.nu_cpf = '{$_SESSION['usucpf']}' ";
		$arrDados = $db->pegaLinha($sql);
	}

	return $arrDados;

}

function pegaDadosIES()
{
	global $db;

	//Recupera os dados de acordo com o CPF do usuário, que deve ser dirigente da instituição
	$sql = "select
				insid,
				inssigla as sigla,
				insnome as nome,
				instelefone as telefone,
				insemail as email,
				insorgacad as organizacao_academica
			from
				snf.instituicaoensino ins
			inner join
				snf.dirigentemaximo dir ON dir.dirid = ins.dirid
			where
				dircpf = '{$_SESSION['usucpf']}'
			and
				dir.dirstatus = 'A'
			and
				ins.insstatus = 'A'";

	$arrDados = $db->pegaLinha($sql);

	if(!$arrDados){
		$sql = "select
				ies.co_ies,
				sg_ies as sigla,
				no_ies as nome,
				--nu_telefone as telefone,
				--no_email as email,
				organizacao_academica as organizacao_academica,
				dir.nu_cpf as cpf_dirigente,
				dir.no_dirigente as nome_dirigente
			from
				emec.ies ies
			inner join
				emec.dirigente dir ON dir.co_ies = ies.co_ies
			inner join
				 emec.organizacao_academica org ON org.tp_organizacao_gn = ies.tp_organizacao_gn
			where
				dir.nu_cpf = '{$_SESSION['usucpf']}' ";
		$arrDados = $db->pegaLinha($sql);
	}

	return $arrDados;

}

function pegaDadosDirigente()
{
	global $db;

	//Recupera os dados de acordo com o CPF do usuário, que deve ser dirigente da instituição
	$sql = "select
				dircpf as cpf,
				dirnome as nome
			from
				snf.dirigentemaximo dir
			inner join
				snf.instituicaoensino ins ON dir.dirid = ins.dirid
			inner join
				snf.mantenedora man ON man.manid = ins.manid
			inner join
				emec.mantenedora emec ON emec.co_mantenedora = man.co_mantenedora
			where
				dir.dircpf = '{$_SESSION['usucpf']}'
			and
				manstatus = 'A'
			and
				insstatus = 'A'
			and
				dirstatus = 'A'";

	$arrDados = $db->pegaLinha($sql);

	if(!$arrDados){
		$sql = "select
				dir.nu_cpf as cpf,
				dir.no_dirigente as nome
			from
				emec.mantenedora man
			inner join
				emec.ies ies ON ies.co_mantenedora = man.co_mantenedora
			inner join
				emec.dirigente dir ON dir.co_ies = ies.co_ies
			where
				dir.nu_cpf = '{$_SESSION['usucpf']}' ";
		$arrDados = $db->pegaLinha($sql);
	}

	return $arrDados;

}

function recuperaArquivoDirigente()
{
	global $db;

	$sql = "select
				arq.*
			from
				snf.dirigentemaximo dir
			inner join
				snf.anexo ane ON ane.aneid = dir.aneid
			inner join
				snf.instituicaoensino ins ON ins.dirid = dir.dirid
			inner join
				public.arquivo arq ON arq.arqid = ane.arqid
			where
				dircpf = '{$_SESSION['usucpf']}'
			and
				dirstatus = 'A'
			and
				anestatus = 'A'";
	return $db->pegaLinha($sql);

}

function insereDadosMantenedora($co_mantenedora)
{
	global $db;

	$sql = "insert into snf.mantenedora (manstatus,co_mantenedora) values ('A',$co_mantenedora) returning manid";
	$manid = $db->pegaUm($sql);
	$_SESSION['snf']['manid'] = $manid;
	$db->commit();

}

function insereDadosInstituicao($arrDados)
{
	global $db;


	$sql = "select
				man_snf.manid,
				dir.nu_cpf,
				dir.no_dirigente
			from
				snf.mantenedora man_snf
			inner join
				emec.mantenedora man ON man.co_mantenedora = man_snf.co_mantenedora
			inner join
				emec.ies ies ON ies.co_mantenedora = man.co_mantenedora
			inner join
				emec.dirigente dir ON dir.co_ies = ies.co_ies
			where
				dir.nu_cpf = '{$_SESSION['usucpf']}' ";

	$arrD = $db->pegaLinha($sql);


	//Insere o Dirigente
	$sql = "select
				dirid
			from
				snf.dirigentemaximo dir
			where
				dircpf = '{$arrD['nu_cpf']}'
			and
				dirstatus = 'A'";
	$dirid = $db->pegaUm($sql);
	if(!$dirid){
		$sql = "insert into snf.dirigentemaximo (dirnome,dircpf,aneid,dirstatus) values ('{$arrD['no_dirigente']}','{$arrD['nu_cpf']}',null,'A') returning dirid";
		$dirid = $db->pegaUm($sql);
	}


	$sql = "insert
				into snf.instituicaoensino
			(co_ies,dirid,manid,insnome,inssigla,instelefone,insemail,insstatus,insorgacad)
				values
			({$arrDados['co_ies']},$dirid,{$arrD['manid']},'{$arrDados['nome']}','{$arrDados['sigla']}','{$arrDados['telefone']}','{$arrDados['email']}','A','{$arrDados['organizacao_academica']}') returning insid";
	$insid = $db->pegaUm($sql);
	$_SESSION['snf']['insid'] = $insid;
	$db->commit();
}

//Função que pega os dados da instituição de acordo com o CPF do usuário, que deve ser um Dirigente Máximo(Reitor)
function pegaDadosInstituicao($usucpf)
{
	global $db;

	//$usucpf = "";

	$sql = "select
				dir.dirnome,
				dir.dircpf,
				dir.dirid,
				dir.carid,
				arq.arqid,
				arq.arqnome || '.' || arq.arqextensao as anexo_dirigente,
				ins.inscnpj,
				ins.insnome,
				ins.inssigla,
				ins.instelefone,
				ins.insfax,
				ins.insemail,
				ende.*,
				mun.mundescricao,
				mun.estuf,
				ins.insid
			from
				snf.dirigentemaximo dir
			--inner join
				--seguranca.usuario usu ON usu.usucpf = dir.dircpf
			left join
				snf.anexo ane ON ane.aneid = dir.aneid and ane.anestatus = 'A'
			left join
				public.arquivo arq ON arq.arqid = ane.arqid
			inner join
				snf.instituicaoensino ins ON ins.insid = dir.insid
			left join
				snf.endereco ende on ende.insid = ins.insid and ende.endstatus = 'A'
			/*left join
				entidade.endereco ende2 on ende2.entid = ins.entid --and ende2.endstatus = 'A'*/
			left join
				territorios.municipio mun on mun.muncod = ende.muncod
			where
				dir.dircpf = '$usucpf'
			and
				dir.dirstatus = 'A'
			and
				ins.insstatus = 'A'";
	//dbg($sql);
	return $db->pegaLinha($sql);

}

function downloadArquivo()
{
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$campos	= array("anedatahora" => "now()","anestatus" => "'A'","anetipo" => "1");
	$file = new FilesSimec("anexo", $campos, "snf");
	$file->getDownloadArquivo($_REQUEST['arqid']);
	exit;
}

function excluirArquivo()
{
	global $db;

	$sql = "update snf.anexo set anestatus = 'I' where arqid = {$_REQUEST['arqid']}";
	$db->executar($sql);
	$db->commit();
	exit;
}

function salvaTermoGerado($stream)
{
	global $db;

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$campos	= array("anedatahora" => "now()","anestatus" => "'A'","anetipo" => "3");
	$file   = new FilesSimec("anexo", $campos, "snf");
	$file->arquivo["name"] = "termo_adesao.pdf";
	$file->setStream("Termo de Adesão",$stream,"application/pdf",".pdf");
	$arqid = $file->getIdArquivo();
	$sql = "select aneid from snf.anexo where arqid = $arqid";
	$aneid = $db->pegaUm($sql);
	$insid = $_SESSION['snf']['insid'];
	$usucpf = $_SESSION['usucpf'];
	$sql = "insert into snf.termoadesao (insid,usucpf,terdatahora,aneid) values ($insid,$usucpf,now(),$aneid);";
	$db->executar($sql);
	$db->commit();
}

function salvarDirigenteMax($insid)
{
	global $db;

	extract($_POST);

	$dircpf = str_replace(array(".","/","-"),array("","",""),$_POST['dircpf']);

	//Inativa os outros dirigentes
	$sql = "update snf.dirigentemaximo set dirstatus = 'I' where insid = $insid";
	$db->executar($sql);

	//Insere o Dirigente
	$sql = "
			select
				dirid
			from
				snf.dirigentemaximo dir
			where
				dircpf = '$dircpf'
			and
				dirstatus = 'A'";
	$dirid = $db->pegaUm($sql);
	if(!$dirid){
		$sql = "insert into snf.dirigentemaximo (dirnome,dircpf,aneid,dirstatus,insid,carid) values ('$dirnome','$dircpf',null,'A',$insid,$carid) returning dirid";
		$dirid = $db->pegaUm($sql);
	}else{
		$sql = "update snf.dirigentemaximo set insid = $insid, carid = $carid, dirstatus = 'A' where dirid = $dirid";
		$db->executar($sql);
	}
	$db->commit();

}

function recuperaDadosDirigente($insid)
{
	global $db;
	$sql = "
			select
				*
			from
				snf.dirigentemaximo dir
			where
				insid = '$insid'
			and
				dirstatus = 'A'";
	return $db->pegaLinha($sql);
}

function recuperaDiretorPorPorCNPJ()
{
	global $db;

	$cnpj = str_replace(array(".","/","-"),array("","",""),$_POST['cnpj']);

	$sql = "select
				dircpf,
				dirnome,
				carid
			from
				snf.dirigentemaximo dir
			inner join
				snf.instituicaoensino ins ON ins.insid = dir.insid
			where
				inscnpj = '$cnpj'
			and
				insstatus = 'A'
			and
				dirstatus = 'A'";
	$arrDados = $db->pegaLinha($sql);

	ob_clean();
	header ("content-type: text/xml");

	if($arrDados){

		$xml = "<DADOS>";
		foreach($arrDados as $campo => $valor){
			$xml.="<$campo>$valor</$campo>";
		}
		$xml.="</DADOS>";
	}else{
		$xml = "<SEMDADOS></SEMDADOS>";
	}
	echo $xml;
	exit;
}

function filtraCurso()
{
	global $db;

	$ateid = $_POST['ateid'];
	$obrigatorio = $_POST['obrigatorio'] ? $_POST['obrigatorio'] : "S";
	if(!$ateid){
		$sql = "	select
						1 as codigo,
						2 as descricao";
		$db->monta_combo("curid",$sql,"N","Selecione...","","","","200",$obrigatorio);
	}else{
		$sql = "select
					curid as codigo,
					curdesc as descricao
				from
					catalogocurso.curso
				where
					curstatus = 'A'
				and
					ateid = $ateid
				order by
					curdesc";
   		$db->monta_combo("curid",$sql,"S","Selecione...","","","","",$obrigatorio);
	}
   	exit;
}

function verificaImportacaoEscolas($arrDados)
{
	global $db;

	//1º Regra: Após o dia 30 de março, não é permitida a importação de escolas
	/*if((int)date("md") > (int)330){
		return true;
	}*/

	//Verifica as escolas de acordo com o estado e/ou município do usuário, para permitir a importação
	//Regra pra importação de escolas: 
	//Todas as escolas do PDE Interativo vinculadas ao estado/município do usuário devem estar no estado do workflow "Em análise no sistema nacional de formação" (esdid = 417)
	if($arrDados['estuf'] && !$arrDados['muncod']){
		$sql = "
			select 	count(distinct pdeid) as qtde_escolas,
					count(distinct curid) as qtde_cursos,
					count(distinct modid) as qtde_modalidades,
					count(distinct pcfano) as qtde_periodos
			from snf.prioridadecursoescola
			where estuf = '{$arrDados['estuf']}' and pdiesfera = 'Estadual' and pristatus = 'A';
		";
		$escolasSNF = $db->pegaLinha($sql);

		$sql = "
		 	Select	escolas as qtde_escolas, 
					cursos  as qtde_cursos, 
					modalidades  as qtde_modalidades, 
					periodos  as qtde_periodos
			FROM dblink('host= user= password= port= dbname=',
		    'select 
				count(distinct pde.pdeid) as qtde_escolas,
				count(distinct cu.curid) as qtde_cursos,
				count(distinct mo.modid) as qtde_modalidades,
				count(distinct pcf.pcfano) as qtde_periodos 
			from pdeinterativo.pdinterativo pde
			inner join pdeinterativo.planoformacaodocente pfd on pfd.pdeid = pde.pdeid
			inner join pdeinterativo.periodocursoformacao  pcf on pcf.pcfid = pfd.pcfid
			inner Join catalogocurso.curso cu on pfd.curid = cu.curid 
			inner Join catalogocurso.nivelcurso nc on nc.ncuid = cu.ncuid 
			inner Join catalogocurso.areatematica art on art.ateid = cu.ateid 
			inner Join catalogocurso.modalidadecurso_curso mc on mc.curid = cu.curid 
			inner Join catalogocurso.modalidadecurso MO on mo.modid = mc.modid
			left join workflow.documento d on pde.formacaodocid = d.docid
			left join workflow.estadodocumento ed on d.esdid = ed.esdid
			
			Where pde.estuf = ''{$arrDados['estuf']}'' and pde.pdiesfera = ''Estadual'' and pde.pdistatus =''A'' and
			d.esdid = ".WORKFLOW_PDE_INTERATIVO_EM_ANALISE_SNF."') as 
			total (escolas integer, 
				  cursos integer, 
				  modalidades integer, 
				  periodos integer
			);	  
  		";

		$escolasPDE = $db->pegaLinha($sql);
		if(!$escolasPDE && !$escolasSNF){//Não existem escolas para o perfil
			exibeErroUsuario("Não exitem escolas em análise pelo Sistema Nacional de Formação no seu Estado.");
		}elseif(!$escolasPDE){ //Não precisa importar nada
			return true;
		}elseif($escolasPDE && $escolasPDE != $escolasSNF){ //Importa as escolas do PDE Interativo
			foreach($escolasPDE as $chave => $valor){
				if($valor > $escolasSNF[$chave]){
					$qtde = $valor - $escolasSNF[$chave];
					$importar = str_replace("qtde_","",$chave);
					switch($importar){
						case "escolas":
							$texto = "$qtde novas Escolas estão sendo importadas";
							break;
						case "cursos":
							$texto = "$qtde novos Cursos estão sendo importados";
							break;
						case "modalidade":
							$texto = "$qtde novas Modalidade estão sendo importadas";
							break;
						case "periodos":
							$texto = "$qtde novos Períodos estão sendo importados";
							break;
					}
					break;
				}else{
					return true;
				}
			}

			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr class="center SubtituloTabela" >
					<td colspan="2">
						<p class="red bold" ><?php echo "Carregando ... $texto para análise pelo Sistema Nacional de Formação no seu Estado." ?></p>
					</td>
				</tr>
			</table>
			<?php
			die("<script>jQuery('#aguarde').hide();importarEscolasPDEInterativo('{$arrDados['estuf']}');</script>");
		}
	}elseif($arrDados['muncod'] && $arrDados['estuf']){
		$sql = "
			select	count(distinct pdeid) as qtde_escolas,
					count(distinct curid) as qtde_cursos,
					count(distinct modid) as qtde_modalidades,
					count(distinct pcfano) as qtde_periodos
			from snf.prioridadecursoescola
			where estuf = '{$arrDados['estuf']}' and muncod = '{$arrDados['muncod']}' and pdiesfera = 'Municipal' and pristatus = 'A'
		";
		$escolasSNF = $db->pegaLinha($sql);
			
		$sql = "			
		 	Select	escolas as qtde_escolas, 
					cursos  as qtde_cursos, 
					modalidades  as qtde_modalidades, 
					periodos  as qtde_periodos
			FROM dblink('host= user= password= port= dbname=',
		    'select 
				count(distinct pde.pdeid) as qtde_escolas,
				count(distinct cu.curid) as qtde_cursos,
				count(distinct mo.modid) as qtde_modalidades,
				count(distinct pcf.pcfano) as qtde_periodos 
			from pdeinterativo.pdinterativo pde
			inner join pdeinterativo.planoformacaodocente pfd on pfd.pdeid = pde.pdeid
			inner join pdeinterativo.periodocursoformacao  pcf on pcf.pcfid = pfd.pcfid
			inner Join catalogocurso.curso cu on pfd.curid = cu.curid 
			inner Join catalogocurso.nivelcurso nc on nc.ncuid = cu.ncuid 
			inner Join catalogocurso.areatematica art on art.ateid = cu.ateid 
			inner Join catalogocurso.modalidadecurso_curso mc on mc.curid = cu.curid 
			inner Join catalogocurso.modalidadecurso MO on mo.modid = mc.modid
			left join workflow.documento d on pde.formacaodocid = d.docid
			left join workflow.estadodocumento ed on d.esdid = ed.esdid
			where pde.estuf = ''{$arrDados['estuf']}'' and pde.muncod = ''{$arrDados['muncod']}'' and pde.pdiesfera = ''Municipal'' and pde.pdistatus = ''A'' and
			d.esdid = ".WORKFLOW_PDE_INTERATIVO_EM_ANALISE_SNF."') as 
			total (escolas integer, 
				  cursos integer, 
				  modalidades integer, 
				  periodos integer
			);
		";

		$escolasPDE = $db->pegaLinha($sql);

		if(!$escolasPDE && !$escolasSNF){//Não existem escolas para o perfil
			exibeErroUsuario("Não exitem escolas em análise pelo Sistema Nacional de Formação no seu Estado.");
		}elseif(!$escolasPDE){ //Não precisa importar nada
			return true;
		}elseif($escolasPDE && $escolasPDE != $escolasSNF){ //Importa as escolas do PDE Interativo
			foreach($escolasPDE as $chave => $valor){
				if($valor > $escolasSNF[$chave]){
					$qtde = $valor - $escolasSNF[$chave];
					$importar = str_replace("qtde_","",$chave);
					switch($importar){
						case "escolas":
							$texto = "$qtde novas Escolas estão sendo importadas";
							break;
						case "cursos":
							$texto = "$qtde novos Cursos estão sendo importados";
							break;
						case "modalidade":
							$texto = "$qtde novas Modalidade estão sendo importadas";
							break;
						case "periodos":
							$texto = "$qtde novos Períodos estão sendo importados";
							break;
					}
					break;
				}else{
					return true;
				}
			}
			?>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr class="center SubtituloTabela" >
					<td colspan="2">
						<p class="red bold" ><?php echo "Carregando ... $texto para análise pelo Sistema Nacional de Formação no seu Município." ?></p>
					</td>
				</tr>
			</table>
			<?php
			die("<script>jQuery('#aguarde').hide();importarEscolasPDEInterativo('{$arrDados['estuf']}','{$arrDados['muncod']}');</script>");
		}else{
			return true;
		}
	}else{
		exibeErroUsuario("O seu perfil não está vinculado a um estado ou município. Favor entrar em contato com o gestor do sistema.");
	}

}

function importarEscolas(){
	global $db;

	$estuf = $_REQUEST['estuf'];
	$muncod = $_REQUEST['muncod'];
	
	$arrDados = recuperaEscolasImportacao($estuf, $muncod);

	if($arrDados){
		foreach($arrDados as $dado){
			//1º - Escolas com IDEB Anos Iniciais (priidebi) E Anos Finais (priidebf) NULOS
			if(!$dado['priidebi'] && !$dado['priidebf'] ){
				$arrPrioridade[0][] = $dado;
			//2º - Escolas com IDEB Anos Iniciais (priidebi) PREENCHIDOS E Anos Finais (priidebf) NULOS
			}elseif($dado['priidebi'] && !$dado['priidebf'] ){
				$arrPrioridade[1][] = $dado;
			//3º - Escolas com IDEB Anos Iniciais (priidebi) NULOS E Anos Finais (priidebf) PREENCHIDOS
			}elseif(!$dado['priidebi'] && $dado['priidebf'] ){
				$arrPrioridade[2][] = $dado;
			//4º - Escolas com IDEB Anos Iniciais (priidebi) E Anos Finais (priidebf) PREENCHIDOS
			}else{
				$arrPrioridade[3][] = $dado;
			}
		}
	}

	//Ordena o array de inserção por prioridade, de acordo com as regras de IDEB
	if($arrPrioridade){
		foreach($arrPrioridade as $arrP){
			if($arrP){
				foreach($arrP as $p){
					$arrInserts[] = $p;
				$n++;
				}
			}
		}
	}
	if($arrInserts){
		foreach($arrInserts as $arrI){
			if($arrI){
				$arrI['pristatus'] = 'A';
				$arrI['privagasprevistas'] = ""; //Inicialmente, o número de vagas previstas é igual ao número de vagas solicitadas.

				if($arrI['curid'] && $arrI['modid'] && $arrI['pcfano']){
					$sql = "
						select 	priordem,
								prdid
						from snf.prioridadecursoescola
						where curid = '{$arrI['curid']}' and modid = '{$arrI['modid']}' and pcfano = '{$arrI['pcfano']}' and pristatus = 'A'
					";
					$arrPri  = $db->pegaLinha($sql);

					//Verifica se já existe prioridade atribuída ao conjunto de cursos, para continuar a partir da existente
					//Se não existir, é 1
					if(!$arrPri['priordem']){
						$arrI['priordem'] = 1;
					}else{
						$arrI['priordem'] = $arrPri['priordem'];
					}
					//CÓDIGO COMENTADO:FOI ALTERADO A REGRA DE GRUPO DE WORKFLOW, DEMANDA SOLICITADA PELO ANALISTA CID, NOVA REGRA IMPLEMENTADA NA FUNÇÃO GERASWOEKFLOW - DATA: 03/05/2012
					//Regra de Grupo de Workflow: As chaves para o WorkFlow são (Curso, Modalidade e Ano) curid, modid e pcfano
					/*if(!$arrPri['prdid']){
						$esdid = $db->pegaUm("SELECT esdid FROM workflow.estadodocumento WHERE tpdid='".WORKFLOW_SNF."' AND esdordem='1'");
						$docid = $db->pegaUm("INSERT INTO workflow.documento(tpdid, esdid, docdsc, docdatainclusao) VALUES ('".WORKFLOW_SNF."','".$esdid."','SNF - Curso: {$arrI['curid']}, Modalidade: {$arrI['modid']}, Ano: {$arrI['pcfano']}', NOW()) RETURNING docid;");
						$prdid = $db->pegaUm("INSERT INTO snf.prioridadedocumento (docid) values ($docid) RETURNING prdid;");
						$db->commit();
						$arrI['prdid'] = $prdid;
					}else{
						$arrI['prdid'] = $arrPri['prdid'];
					}*/
				}
				unset($arrChave);
				unset($arrValor);
				foreach($arrI as $chave => $valor){
					$arrChave[] = $chave;
					$arrValor[] = $valor ? "'".$valor."'" : "NULL" ;
				}
				$sqlI = "insert into
						snf.prioridadecursoescola
					(".implode(",",$arrChave).")
						values
					(".implode(",",$arrValor).");";
				$db->executar($sqlI);
				$db->commit();
			}

		}
	}
	ob_clean();
	echo "ok";
	exit;
}

function recuperaEscolasImportacao($estuf, $muncod = null){
	global $db;
	
	$esfera = $_SESSION['snf']['esfera'];
	
	if ($esfera == 'Municipal'){
		$where .= " and pd.muncod = '$muncod'";
		$whereDBlink .= " and pd.muncod = ''$muncod''";
		$intesfera = "M";
	}elseif ($esfera == 'Estadual'){
		$where .= " and pd.estuf = '$estuf'";
		$whereDBlink .= " and pd.estuf = ''$estuf''";
		$intesfera = "E";
	}
	$where .= " and pd.pdiesfera = '$esfera'";
	$whereDBlink .= " and pd.pdiesfera = ''$esfera''";
	
	$sql = "
			select 	distinct 
					pdeid, pdicodinep,
					pdenome, pdiesfera,
					estuf, muncod,
					pdilocalizacao,
					ateid, atedesc,
					curid, curdesc,
					ncuid, ncudesc,
					modid, moddesc,
					pcfid, pcfano,
					COUNT(*) as privagassolicitadas,
					pridemsoc,
					pridemsocvagassolicitadas
					priidebi,
					priidebf
			From dblink('host= user= password= port= dbname=',
			'select distinct
					PD.pdeid, PD.pdicodinep,
					PD.pdenome, PD.pdiesfera,
					PD.estuf, PD.muncod,
					PD.pdilocalizacao,
					AT.ateid, AT.atedesc,
					CU.curid, CU.curdesc,
					NC.ncuid, NC.ncudesc,
					MO.modid, MO.moddesc,
					PCF.pcfid, PCF.pcfano,
					COUNT(*) as privagassolicitadas,        
					CASE WHEN
                        (SELECT CAST(dem.curid AS varchar) from catalogocurso.cursodemandasocial dem where dem.curid = CU.curid limit 1) IS NOT NULL THEN ''S''
					ELSE ''N''
					END as pridemsoc,
					(select count(distinct mem.mdsid) from catalogocurso.membrosdemandasocial mem
             		where mem.curid = CU.curid and mem.pcfid = PCF.pcfid and mem.modid = MO.modid and mem.pdeid = PD.pdeid and cpdstatus = ''A'') as pridemsocvagassolicitadas,
					
		            ID.intvalor as priidebi,
		            ID1.intvalor as priidebf
			from pdeinterativo.pdinterativo pd
			inner join pdeinterativo.planoformacaodocente PFD on PD.pdeid = PFD.pdeid
			inner join catalogocurso.curso CU on PFD.curid = CU.curid
			inner join catalogocurso.nivelcurso NC on NC.ncuid = CU.ncuid
			inner join catalogocurso.areatematica AT on AT.ateid = CU.ateid
			inner join catalogocurso.modalidadecurso_curso MC on MC.curid = CU.curid
			inner join catalogocurso.modalidadecurso MO on MO.modid = MC.modid
			inner join workflow.documento d on pd.formacaodocid = d.docid
			inner join pdeinterativo.periodocursoformacao PCF on PCF.pcfid = PFD.pcfid
			inner join workflow.estadodocumento ed on d.esdid = ed.esdid

			LEFT JOIN pdeinterativo.indicadorestaxas ID on CAST(PD.pdicodinep AS INTEGER) = CAST(ID.intinep AS INTEGER)
			AND ID.intano = 2009 AND ID.intsubmodulo = ''I''
			AND ID.intensino = ''I''
			AND ID.intesfera = ''$intesfera''
			
			LEFT JOIN pdeinterativo.indicadorestaxas ID1 on CAST(PD.pdicodinep AS INTEGER) = CAST(ID1.intinep AS INTEGER)
			AND ID1.intano = 2009
			AND ID1.intsubmodulo = ''I''
			AND ID1.intensino = ''F''
			AND ID.intesfera = ''$intesfera''

			WHERE ED.ESDID = ".WORKFLOW_PDE_INTERATIVO_EM_ANALISE_SNF." $whereDBlink and pd.pdistatus = ''A''

			GROUP BY
					PD.pdeid, PD.pdicodinep,
					PD.pdenome, PD.pdiesfera,
					PD.estuf, PD.muncod,
					PD.pdilocalizacao,
					AT.ateid, AT.atedesc,
					CU.curid, CU.curdesc,
					NC.ncuid, NC.ncudesc,
					MO.modid, MO.moddesc,
					PCF.pcfid, PCF.pcfano,
					pridemsoc,
					ID.intvalor,
					ID1.intvalor') AS pd (
				            pdeid integer, pdicodinep varchar,
				            pdenome varchar, pdiesfera varchar,
				            estuf varchar, muncod varchar,
				            pdilocalizacao varchar,
				            ateid integer, atedesc varchar,
				            curid integer, curdesc varchar,
				            ncuid integer, ncudesc varchar,
				            modid integer, moddesc varchar,
				            pcfid integer, pcfano integer,
				            privagassolicitadas integer,
				            pridemsoc varchar,
				            pridemsocvagassolicitadas integer,
				            priidebi integer,
				            priidebf integer)
			WHERE PDICODINEP NOT IN (SELECT distinct pdicodinep from snf.prioridadecursoescola)

			GROUP BY
			pdeid, pdicodinep,
			pdenome, pdiesfera,
			estuf, muncod,
			pdilocalizacao,
			ateid, atedesc,
			curid, curdesc,
			ncuid, ncudesc,
			modid, moddesc,
			pcfid, pcfano,
			pridemsoc,
			pridemsocvagassolicitadas,
			priidebi,
			priidebf
	";
	return $db->carregar($sql);
}

function recuperaEstadoMunicipioPerfil()
{
	global $db;

	$usucpf = $_SESSION['usucpf'];

	$arrPerfil = pegaPerfilGeral();

	if(in_array(PERFIL_EQUIPE_ESTADUAL_APROVACAO, $arrPerfil)){
		$pflcod = PERFIL_EQUIPE_ESTADUAL_APROVACAO;
		$campo = "estuf";
	}elseif(in_array(PERFIL_EQUIPE_ESTADUAL,$arrPerfil)){
		$pflcod = PERFIL_EQUIPE_ESTADUAL;
		$campo = "estuf";
	}elseif(in_array(PERFIL_EQUIPE_MUNICIPAL_APROVACAO,$arrPerfil)){
		$pflcod = PERFIL_EQUIPE_MUNICIPAL_APROVACAO;
		$campo = "muncod";
	}elseif(in_array(PERFIL_EQUIPE_MUNICIPAL,$arrPerfil)){
		$pflcod = PERFIL_EQUIPE_MUNICIPAL;
		$campo = "muncod";
	}else{
		//return array("muncod" => "", "estuf" => "MG");
		//return array("muncod" => "3103405","estuf" => "MG");
		return false;
	}
	if($campo == "muncod"){
		$sql = "
			select 	rpu.$campo,
					mun.estuf
			From snf.usuarioresponsabilidade rpu
			Inner Join territorios.municipio mun ON mun.muncod = rpu.muncod
			Where rpu.usucpf = '$usucpf' and rpu.rpustatus = 'A' and rpu.pflcod = $pflcod
		";
	}else{
		$sql = "
			Select $campo
			From snf.usuarioresponsabilidade
			Where usucpf = '$usucpf' and rpustatus = 'A' and pflcod = $pflcod
		";
	}
	
	return $db->pegaLinha($sql);
}

function exibeErroUsuario($texto)
{
	global $db;
	?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr class="center SubtituloTabela" >
			<td colspan="2">
				<p class="red bold" ><?php echo $texto ?></p>
			</td>
		</tr>
	</table>
	<?php
	die("<script>jQuery('#aguarde').hide();</script>");

}

function enviaEmailAdministradores($insid)
{
	global $db;

	$sql = "select
				insnome,
				inscnpj
			from
				snf.instituicaoensino
			where
				insid = $insid";
	$arrDados = $db->pegaLinha($sql);

	$sql = "select distinct
				usuemail
			from
				seguranca.perfilusuario pu
			inner join
				seguranca.perfil p on p.pflcod = pu.pflcod
			inner join
				seguranca.usuario usu ON usu.usucpf = pu.usucpf
			and
				p.sisid = {$_SESSION['sisid']}
			and
				pflstatus = 'A'
			and
				p.pflcod  = ".PERFIL_ADMINISTRADOR;

	$arrUsu = $db->carregarColuna($sql);

	$arrUsu[] = "julianomeinen.souza@gmail.com";

	if($arrUsu && $arrDados && !strstr($_SERVER['SERVER_NAME'],"simec-local")){
		include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';
		$e = new EmailAgendado();
		$e->setTitle("Programa de Formação Continuada");
		$html = 'Senhor(a) Administrador(a),<br /><br />
		 			A instituição de ensino '.$arrDados['insnome'].', inscrita sob o CNPJ '.mascaraglobalTermoAdesao($arrDados['inscnpj'],"##.###.###/####-##").' aderiu ao Programa de Formação Continuada através do sistema SNF. <br />
		 			Favor Verificar.';
		$e->setText($html);
		$e->setName("Programa de Formação Continuada");
		$e->setEmailOrigem("no-reply@mec.gov.br");
		$e->setEmailsDestino($arrUsu);
		$e->enviarEmails();
	}

}

function pesquisarCursos()
{

}

function exibeCursosSNF($estuf, $muncod = null){
	global $db;

	$estadoAtual = $_SESSION['estadoAtual'];
	
	$modid = $_REQUEST['modid'];
	$pcfid = $_REQUEST['pcfid'];
	$ncuid = $_REQUEST['ncuid'];

	if(!$pcfid){
		?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" height="100%" >
			<tr class="center SubtituloTabela" >
				<td colspan="2">
					Favor selecionar o filtro de Período.
				</td>
			</tr>
		</table>
		<?php
	}else{
		$arrWhere[] = "pri.estuf = '$estuf'";
		$arrWhere_p[] = "p.estuf = '$estuf'";
		
		if($muncod){
			$arrWhere[] = "pri.muncod = '$muncod'";
			$arrWhere[] = "pdiesfera = 'Municipal'";
			$arrWhere_p[] = "p.muncod = '$muncod'";
			$arrWhere_p[] = "pdiesfera = 'Municipal'";
		}else{
			$arrWhere[] = "pdiesfera = 'Estadual'";
			$arrWhere_p[] = "pdiesfera = 'Estadual'";
		}

		if($_POST['ateid']){
			$arrWhere[] = "ateid = '{$_POST['ateid']}'";
		}
		if($_POST['curid']){
			$arrWhere[] = "curid = '{$_POST['curid']}'";
		}
		if($modid){
			$arrWhere[] = "modid = '{$modid}'";
		}
		if($ncuid){
			$arrWhere[] = "ncuid = '{$ncuid}'";
		}

		$arrCabecalho = array("Ação","Área Temática","Curso","Qtde de Escolas Solicitantes","Qtde de Vagas Solicitadas","Modalidade","Nível");

		$sqlLista = "select distinct
					'<img src=\"../imagens/mais.gif\" id=\"img_mais_curid_' || curid || pri.modid || '\" class=\"link img_middle\" title=\"Visualizar Escolas\" onclick=\"expandirEscolas(' || curid || ',' || pri.modid || ');\"  /><img src=\"../imagens/menos.gif\" id=\"img_menos_curid_' || curid || pri.modid || '\" style=\"display:none\"  class=\"link\" title=\"Esconder Escolas\" onclick=\"esconderEscolas(' || curid || pri.modid || ')\"  />' as acao,
					atedesc,
					'<span title=\"Ver Informações do Curso.\" style=\"cursor:pointer\" onclick=\"popupCurso(' || curid || ');\">' || curdesc || '</span>' as curdesc,
					--count( distinct pdeid) as qtde_escolas,
					--sum(privagassolicitadas) as qtde_solicitacoes,
					(CASE WHEN pri.pcfid in(1,2) THEN
						(select count(p.pdeid) from snf.prioridadecursoescola p
						 where p.curid = pri.curid and p.pcfid in (1,2)
						 and pristatus = 'A' ".($arrWhere_p ? " and ".implode(" and ",$arrWhere_p) : "")."
						)
					      ELSE
						(select count(p.pdeid) from snf.prioridadecursoescola p
						 where p.curid = pri.curid and p.pcfid = pri.pcfid
						 and pristatus = 'A' ".($arrWhere_p ? " and ".implode(" and ",$arrWhere_p) : "")."
						)
					END) AS qtde_escolas,
					(CASE WHEN pri.pcfid in(1,2) THEN
						(select sum(p.privagassolicitadas) from snf.prioridadecursoescola p
						 where p.curid = pri.curid and p.pcfid in (1,2)
						 and pristatus = 'A' ".($arrWhere_p ? " and ".implode(" and ",$arrWhere_p) : "")."
						)
					      ELSE
						(select sum(p.privagassolicitadas) from snf.prioridadecursoescola p
						 where p.curid = pri.curid and p.pcfid = pri.pcfid
						 and pristatus = 'A' ".($arrWhere_p ? " and ".implode(" and ",$arrWhere_p) : "")."
						)
					END) AS qtde_solicitacoes,
					moddesc,
					ncudesc,
					--docid,
					'</tr><tr id=\"tr_curid_' || curid || pri.modid || '\" style=\"display:none\" ><td></td><td colspan=\"6\" id=\"td_curid_' || curid || pri.modid || '\" ></td>'
				from
					snf.prioridadecursoescola pri
				left join
					snf.prioridadedocumento prd ON prd.prdid = pri.prdid
				where
					pcfid in($pcfid)
				and
					pristatus = 'A'
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				group by
					atedesc,
					curid,
					curdesc,
					moddesc,
					ncudesc,
					--docid,
					prd.prdid,
					pri.pcfid,
					pri.modid
					--prddescordem
				order by
					".($_POST['mais_solicitados'] ? "qtde_solicitacoes desc," : "")."
					atedesc,
					curdesc,
					qtde_escolas";

		/*$sql = "
		 	select distinct
				'<img src=\"../imagens/mais.gif\" id=\"img_mais_curid_' || curid || '\" class=\"link img_middle\" title=\"Visualizar Escolas\" onclick=\"expandirEscolas(' || curid || ')\"  /><img src=\"../imagens/menos.gif\" id=\"img_menos_curid_' || curid || '\" style=\"display:none\"  class=\"link\" title=\"Esconder Escolas\" onclick=\"esconderEscolas(' || curid || ')\"  />' as acao,
				atedesc,
				curdesc,
				count( distinct pdeid) as qtde_escolas,
				sum(privagassolicitadas) as qtde_solicitacoes,
				moddesc,
				ncudesc,
				docid,
				'</tr><tr id=\"tr_curid_' || curid || '\" style=\"display:none\" ><td></td><td colspan=\"6\" id=\"td_curid_' || curid || '\" ></td>'
			from nf.prioridadecursoescola pri
			left join snf.prioridadedocumento prd ON prd.prdid = pri.prdid
			where pcfid in($pcfid) and pristatus = 'A' ".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			group by atedesc, curid, curdesc, moddesc, ncudesc, docid, prd.prdid
				--prddescordem
			order by ".($_POST['mais_solicitados'] ? "qtde_solicitacoes desc," : "")." atedesc, curdesc, qtde_escolas
		";
		//dbg(simec_htmlentities($sql),1);

		$arrDados = $db->carregar($sql);
		$arrDados = !$arrDados ? array() : $arrDados;
		foreach($arrDados as $i => $dados){
			$arrDocid[] = $dados['docid'];
			unset($arrDados[$i]['docid']);
		}*/
		//dbg($sqlLista,d);
				
		$db->monta_lista($sqlLista, $arrCabecalho, 100, 10, "S", "center", "N");

		if($estadoAtual != WORKFLOW_SNF_ENVIADO_AO_FORUM){
			$perfil = carregaPerfil();
			$estado = recuperaEstadoMunicipioPerfil();
			
			if(	($estado['estuf'] == 'AL') &&
				(
					in_array(PERFIL_EQUIPE_MUNICIPAL, $perfil) || 
					in_array(PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
					in_array(PERFIL_EQUIPE_ESTADUAL, $perfil) || 
					in_array(PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil)
				)
			){
		?>
			<table id="tbl_btn_salvar" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td class="SubtituloTabela center" colspan="2"  >
						<input type="button" name="btn_salvar" value="Salvar" onclick="salvarPrioridade()">
					</td>
				</tr>
			</table>
		<?php
			}
		}
	}
}

function exibeDemandaSocial($estuf,$muncod = null){
	global $db;

	$estadoAtual = $_SESSION['estadoAtual'];
	
	$modid = $_REQUEST['modid'];
	$pcfid = $_REQUEST['pcfid'];
	$ncuid = $_REQUEST['ncuid'];

	if(!$pcfid){
		?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" height="100%" >
			<tr class="center SubtituloTabela" >
				<td colspan="2">
					Favor selecionar o filtro de Período.
				</td>
			</tr>
		</table>
		<?php
	}else{

		$arrWhere[] = "pri.estuf = '$estuf'";
		$arrWhere_p[] = "p.estuf = '$estuf'";

		if($muncod){
			$arrWhere[] = "pri.muncod = '$muncod'";
			$arrWhere[] = "pdiesfera = 'Municipal'";
			$arrWhere_p[] = "p.muncod = '$muncod'";
			$arrWhere_p[] = "pdiesfera = 'Municipal'";
		}else{
			$arrWhere[] = "pdiesfera = 'Estadual'";
			$arrWhere_p[] = "pdiesfera = 'Estadual'";
		}

		if($_POST['ateid']){
			$arrWhere[] = "ateid = '{$_POST['ateid']}'";
		}
		if($_POST['curid']){
			$arrWhere[] = "curid = '{$_POST['curid']}'";
		}
		if($modid){
			$arrWhere[] = "modid = '{$modid}'";
		}
		if($ncuid){
			$arrWhere[] = "ncuid = '{$ncuid}'";
		}

		$arrCabecalho = array("Ação","Área Temática","Curso","Qtde de Escolas Solicitantes","Qtde de Vagas Solicitadas","Qtde Permitida","Modalidade","Nível");

		$sqlLista = "select distinct
					'<img src=\"../imagens/mais.gif\" id=\"img_mais_curid_' || curid || '\" class=\"link img_middle\" title=\"Visualizar Escolas\" onclick=\"expandirEscolas(' || curid || ')\"  /><img src=\"../imagens/menos.gif\" id=\"img_menos_curid_' || curid || '\" style=\"display:none\"  class=\"link\" title=\"Esconder Escolas\" onclick=\"esconderEscolas(' || curid || ')\"  />
					<input type=\"hidden\" id=\"vagas_permitidas_' || curid || '\" name=\"qtde_permitida[]\" value=\"' || floor((select COALESCE(incperdemsocial/100::float,0) from snf.informacaocurso inf where inf.curid = pri.curid limit 1)*COALESCE(sum(privagasprevistas),0)) || '\" />' as acao,
					atedesc,
					'<span title=\"Ver Informações do Curso.\" style=\"cursor:pointer\" onclick=\"popupCurso(\''|| curid ||'\');\">' || curdesc || '</span>' as curdesc,
					--count( distinct pdeid) as qtde_escolas,
					--COALESCE(sum(pridemsocvagassolicitadas),0) as qtde_solicitacoes,
					(CASE WHEN pri.pcfid in(1,2) THEN
						(select count(p.pdeid) from snf.prioridadecursoescola p
						 where p.curid = pri.curid and p.pcfid in (1,2)
						 and pristatus = 'A' and pridemsoc = 'S' ".($arrWhere_p ? " and ".implode(" and ",$arrWhere_p) : "")."
						)
					      ELSE
						(select count(p.pdeid) from snf.prioridadecursoescola p
						 where p.curid = pri.curid and p.pcfid = pri.pcfid
						 and pristatus = 'A' and pridemsoc = 'S' ".($arrWhere_p ? " and ".implode(" and ",$arrWhere_p) : "")."
						)
					END) AS qtde_escolas,
					(CASE WHEN pri.pcfid in(1,2) THEN
						(select COALESCE(sum(p.pridemsocvagassolicitadas),0) from snf.prioridadecursoescola p
						 where p.curid = pri.curid and p.pcfid in (1,2)
						 and pristatus = 'A' and pridemsoc = 'S' ".($arrWhere_p ? " and ".implode(" and ",$arrWhere_p) : "")."
						)
					      ELSE
						(select COALESCE(sum(p.pridemsocvagassolicitadas),0) from snf.prioridadecursoescola p
						 where p.curid = pri.curid and p.pcfid = pri.pcfid
						 and pristatus = 'A' and pridemsoc = 'S' ".($arrWhere_p ? " and ".implode(" and ",$arrWhere_p) : "")."
						)
					END) AS qtde_solicitacoes,
					floor((select COALESCE(incperdemsocial/100::float,0) from snf.informacaocurso inf where inf.curid = pri.curid limit 1)*COALESCE(sum(privagasprevistas),0)) as permitida,
					moddesc,
					ncudesc,
					--docid,
					'</tr><tr id=\"tr_curid_' || curid || '\" style=\"display:none\" ><td></td><td colspan=\"6\" id=\"td_curid_' || curid || '\" ></td>'
				from
					snf.prioridadecursoescola pri
				left join
					snf.prioridadedocumento prd ON prd.prdid = pri.prdid
				where
					pcfid in($pcfid)
				and
					pristatus = 'A'
				and
					pridemsoc = 'S'
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				group by
					atedesc,
					curid,
					curdesc,
					moddesc,
					ncudesc,
					docid,
					prd.prdid,
					pri.pcfid
					--prddescordem
				order by
					".($_POST['mais_solicitados'] ? "qtde_solicitacoes desc," : "")."
					atedesc,
					curdesc,
					qtde_escolas";

		$sql = "select distinct
					'<img src=\"../imagens/mais.gif\" id=\"img_mais_curid_' || curid || '\" class=\"link img_middle\" title=\"Visualizar Escolas\" onclick=\"expandirEscolas(' || curid || ')\"  /><img src=\"../imagens/menos.gif\" id=\"img_menos_curid_' || curid || '\" style=\"display:none\"  class=\"link\" title=\"Esconder Escolas\" onclick=\"esconderEscolas(' || curid || ')\"  />
					<input type=\"hidden\" id=\"vagas_permitidas_' || curid || '\" name=\"qtde_permitida[]\" value=\"' || floor((select COALESCE(incperdemsocial/100::float,0) from snf.informacaocurso inf where inf.curid = pri.curid limit 1)*COALESCE(sum(privagasprevistas),0)) || '\" />' as acao,
					atedesc,
					curdesc,
					count( distinct pdeid) as qtde_escolas,
					COALESCE(sum(pridemsocvagassolicitadas),0) as qtde_solicitacoes,
					floor((select COALESCE(incperdemsocial/100::float,0) from snf.informacaocurso inf where inf.curid = pri.curid limit 1)*COALESCE(sum(privagasprevistas),0)) as permitida,
					moddesc,
					ncudesc,
					docid,
					'</tr><tr id=\"tr_curid_' || curid || '\" style=\"display:none\" ><td></td><td colspan=\"6\" id=\"td_curid_' || curid || '\" ></td>'
				from
					snf.prioridadecursoescola pri
				left join
					snf.prioridadedocumento prd ON prd.prdid = pri.prdid
				where
					pcfid in($pcfid)
				and
					pristatus = 'A'
				and
					pridemsoc = 'S'
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
				group by
					atedesc,
					curid,
					curdesc,
					moddesc,
					ncudesc,
					docid,
					prd.prdid
					--prddescordem
				order by
					".($_POST['mais_solicitados'] ? "qtde_solicitacoes desc," : "")."
					atedesc,
					curdesc,
					qtde_escolas";
		//dbg(simec_htmlentities($sql));
		$arrDados = $db->carregar($sql);
		$arrDados = !$arrDados ? array() : $arrDados;

		foreach($arrDados as $i => $dados){
			$arrDocid[] = $dados['docid'];
			unset($arrDados[$i]['docid']);
		}

		//$db->monta_lista($arrDados,$arrCabecalho,100,10,"N","center","N");
		//dbg($sqlLista,1);
		$db->monta_lista($sqlLista,$arrCabecalho,100,10,"N","center","N");

		if(count($arrDados) > 0){
			if($estadoAtual != WORKFLOW_SNF_ENVIADO_AO_FORUM){
				$perfil = carregaPerfil();
				$estado = recuperaEstadoMunicipioPerfil();
				
				if(	($estado['estuf'] == 'AL') &&
					(
						in_array(PERFIL_EQUIPE_MUNICIPAL, $perfil) || 
						in_array(PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
						in_array(PERFIL_EQUIPE_ESTADUAL, $perfil) || 
						in_array(PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil)
					)
				){				
				
			?>
			<table id="tbl_btn_salvar" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td class="SubtituloTabela center" colspan="2"  >
						<input type="button" name="btn_salvar" value="Salvar" onclick="salvarDemandaSocial()">
					</td>
				</tr>
			</table>
		<?php 
				}
			}
		}
		return $arrDocid ? array_unique($arrDocid) : array();
	}
}


function exibirEscolasPorCurso()
{
	global $db;

	$pcfid = $_REQUEST['pcfid'];
	$arrAno = explode(",",$pcfid);
	if($arrAno){
		foreach($arrAno as $ano){
			exibirEscolasPorAno($ano);
		}
	}else{
		echo "<center>Não existem registros.</center>";
	}
	exit;
}

function exibirEscolasDemandaSocial()
{
	global $db;

	$pcfid = $_REQUEST['pcfid'];
	$arrAno = explode(",",$pcfid);
	if($arrAno){
		foreach($arrAno as $ano){
			exibirEscolasDemandaSocialPorAno($ano);
		}
	}else{
		echo "<center>Não existem registros.</center>";
	}
	exit;
}


function exibirEscolasPorAno($ano){
	global $db;

	$estadoAtual = $_SESSION['estadoAtual'];
	
	extract($_POST);

	$modid = $_POST['modid'];
	$ncuid = $_POST['ncuid'];
	$curid = $_POST['curid'];
	$ordem = $_POST['ordem'];
	if(!$ano){
		$pcfid = $_POST['pcfid'];;
	}else{
		$pcfid = $ano;
	}

	$arrWhere[] = "pri.estuf = '$estuf'";
	
	if($muncod){
		$arrWhere[] = "pri.muncod = '$muncod'";
	}
	$arrWhere[] = "pdiesfera = '".$_SESSION['snf']['esfera']."'";

	if($_POST['ateid']){
		$arrWhere[] = "ateid = '{$_POST['ateid']}'";
	}
	if($_POST['curid']){
		$arrWhere[] = "curid = '{$_POST['curid']}'";
	}
	if($modid){
		$arrWhere[] = "modid = '{$modid}'";
	}
	if($ncuid){
		$arrWhere[] = "ncuid = '{$ncuid}'";
	}

	if($ordem && $ordem_ano == $pcfid){
		salvarTipoOdemCurso($pcfid,$ordem);
		switch($ordem){
			case "idebai":
				$OrderBy = "priidebi";
				break;
			case "idebaf":
				$OrderBy = "priidebf";
				break;
			default:
				$OrderBy = "pdicodinep";
		}
	}else{
		$sql = "Select	prddescordem
				from snf.prioridadedocumento prd
				inner join snf.prioridadecursoescola pri ON pri.prdid = prd.prdid
				where pristatus = 'A' and pcfid in ($pcfid)
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");
		$ordem = $db->pegaUm($sql);
		switch($ordem){
			case "I":
				$OrderBy = "priidebi";
				break;
			case "F":
				$OrderBy = "priidebf";
				break;
			default:
				$OrderBy = "pdicodinep";
		}
	}


	$arrCabecalho = array("Ação", "Cód. INEP", "Escola", "Prioridade", "Plano da Escola (Vagas Solicitadas)", "Plano Autorizado (Vagas Autorizadas)", "UF", "Município", "Localização", "Ano do Curso", "Modalidade", "IDEB AI", "IDEB AF");

	if(($pcfid == 1 || $pcfid == 2) && $estadoAtual != WORKFLOW_SNF_ENVIADO_AO_FORUM){

		$sql = "SELECT 
					'<center><img src=\"../imagens/seta_cima.gif\" title=\"Aumentar Prioridade\" class=\"link img_middle\" onclick=\"aumentarPrioridade(' || curid || ',' || pdeid || ',' || pcfid || ')\"  /><img src=\"../imagens/seta_baixo.gif\" title=\"Reduzir Prioridade\" class=\"link img_middle\" onclick=\"reduzirPrioridade(' || curid || ',' || pdeid || ',' || pcfid || ')\"  /></center>' as acao,
					pdicodinep,
					pdenome,
					'<center><input type=\"text\" size=\"8\" name=\"prioridade[' || curid || '][' || pcfid || '][' || pdeid || '][' || modid || ']\" value=\"' || priordem || '\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" class=\"normal\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);\" onchange=\"alteraPrioridade('|| curid ||',' || pdeid || ',' || pcfid || ')\"  /></center>' as prioridade,
					'<input type=\"hidden\" id=\"vagas_' || curid || '_' || pcfid || '_' || pdeid || '_' || modid ||'\" name=\"vagas[' || curid || '][' || pcfid || '][' || pdeid || '][' || modid || ']\" value=\"' || privagassolicitadas || '\"/> <center>' || privagassolicitadas || '</center>' as privagassolicitadas,
					'<center><input type=\"text\" size=\"8\" id=\"previstas_' || curid || '_' || pcfid || '_' || pdeid || '_' || modid ||'\" name=\"vagas_previstas[' || curid || '][' || pcfid || '][' || pdeid || '][' || modid || ']\" value=\"' || COALESCE(privagasprevistas::text,'') || '\" onblur=\"MouseBlur(this);verificaTotal('|| curid ||','|| pcfid ||','|| pdeid ||')\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" class=\"normal\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);\" /></center>' as vagas,
					pri.estuf,
					mun.mundescricao,
					pdilocalizacao,
					'<input type=\"hidden\" id=\"pcfano_' || curid || '_' || pcfid || '_' || pdeid || '_' || modid ||'\" name=\"pcfano[' || curid || '][' || pcfid || '][' || pdeid || '][' || modid || ']\" value=\"' || pcfano || '\"/>' || pcfano || '' as pcfano,
					'<input type=\"hidden\" id=\"modid_' || curid || '_' || pcfid || '_' || pdeid || '_' || modid ||'\" name=\"modid[' || curid || '][' || pcfid || '][' || pdeid || '][' || modid || ']\" value=\"' || modid || '\"/>' || modid || '' as modid,
					'<span style=\"display:none\">' || coalesce(priidebi,0) || '</span>' || coalesce(priidebi::text,'') as priidebi,
					'<span style=\"display:none\">' || coalesce(priidebf,0) || '</span>' || coalesce(priidebf::text,'') as priidebf
				 FROM
					snf.prioridadecursoescola pri
			LEFT JOIN
					snf.prioridadedocumento prd ON prd.prdid = pri.prdid
		   INNER JOIN
					territorios.municipio mun ON mun.muncod = pri.muncod
				WHERE
					pcfid = $pcfid
				  AND
					pristatus = 'A'
					".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			 ORDER BY
					$OrderBy";
	}else{
		$sql = "SELECT
					'' as acao,
					pdicodinep,
					pdenome,
					'<center>' || priordem || '</center>' as prioridade,
					'<center>' || privagassolicitadas || '</center>' as privagassolicitadas,
					'<center>' || COALESCE(privagasprevistas::text,'') || '</center>' as vagas,
					pri.estuf,
					mundescricao,
					pdilocalizacao,
					pcfano,
					modid,
					'<span style=\"display:none\">' || coalesce(priidebi,0) || '</span>' || coalesce(priidebi::text,'') as priidebi,
					'<span style=\"display:none\">' || coalesce(priidebf,0) || '</span>' || coalesce(priidebf::text,'') as priidebf
				 FROM
					snf.prioridadecursoescola pri
			LEFT JOIN
					snf.prioridadedocumento prd ON prd.prdid = pri.prdid
		   INNER JOIN
					territorios.municipio mun ON mun.muncod = pri.muncod
				WHERE
					pcfid = $pcfid
				  AND
					pristatus = 'A'
					".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			 ORDER BY
					$OrderBy";
	}
	//dbg($sql, 1);
	$arrDados = $db->carregar($sql);
	if($arrDados){
		$sql = "select pcfano from snf.prioridadecursoescola where pcfid = $pcfid limit 1";
		$pcfano = $db->pegaUm($sql);
		switch($OrderBy){
			case "priidebi":
				$selected_ai = "selected='selected'";
				$selected_af = "";
				$selected_p = "";
				break;
			case "priidebf":
				$selected_af = "selected='selected'";
				$selected_ai = "";
				$selected_p = "";
				break;
			default:
				$selected_p = "selected='selected'";
				$selected_af = "";
				$selected_ai = "";
				break;
		}
		echo "<fieldset class=\"field_ano\" ><legend>$pcfano - Ordem: <select onchange=\"alteraPrioridadeCursoCombo($curid,$pcfid) \" id=\"cmb_ordem_curid_{$curid}_pcfid_$pcfid\" ><option $selected_ai value=\"idebai\" >IDEB AI</option><option $selected_af value=\"idebaf\" >IDEB AF</option><option $selected_p value=\"personalizado\" >Personalizado</option></select></legend>";
		switch($OrderBy){
			case "priidebi":
				$ordem = "idebai";
				break;
			case "priidebf":
				$ordem = "idebaf";
				break;
			default:
				$ordem = "personalizado";
		}
		echo "<input type=\"hidden\" id=\"hdn_ordem_curid_{$curid}_pcfid_{$pcfid}\" value=\"$ordem\"  />";
		$arrTamanhoTd = array("5","5","45","5","5","5","5","5","5","5","5","5");
		$db->monta_lista_simples($arrDados,$arrCabecalho,1000000,10000,"N","100%","N",false,$arrTamanhoTd);
		echo "</fieldset>";
	}
}


function exibirEscolasDemandaSocialPorAno($ano)
{
	global $db;

	extract($_POST);

	$modid = $_POST['modid'];
	$ncuid = $_POST['ncuid'];
	$curid = $_POST['curid'];
	$ordem = $_POST['ordem'];
	$pcfid = $ano;

	$arrWhere[] = "pri.estuf = '$estuf'";

	if($muncod){
		$arrWhere[] = "pri.muncod = '$muncod'";
		$arrWhere[] = "pdiesfera = 'Municipal'";
	}else{
		$arrWhere[] = "pdiesfera = 'Estadual'";
	}

	if($_POST['ateid']){
		$arrWhere[] = "ateid = '{$_POST['ateid']}'";
	}
	if($_POST['curid']){
		$arrWhere[] = "curid = '{$_POST['curid']}'";
	}
	if($modid){
		$arrWhere[] = "modid = '{$modid}'";
	}
	if($ncuid){
		$arrWhere[] = "ncuid = '{$ncuid}'";
	}

	$sql = "select
				prddescordem
			from
				snf.prioridadedocumento prd
			inner join
				snf.prioridadecursoescola pri ON pri.prdid = prd.prdid
			where
				pcfid = $pcfid
			and
				pristatus = 'A'
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");
	$ordem = $db->pegaUm($sql);
	switch($ordem){
		case "I":
			$OrderBy = "priidebi";
			break;
		case "F":
			$OrderBy = "priidebf";
			break;
		default:
			$OrderBy = "priordem";
	}

	$arrCabecalho = array("Cód. INEP","Escola","Plano da Escola (Vagas Solicitadas)","Plano Autorizado (Vagas Autorizadas)","UF","Município","Localização");

	$sql = "select
				pdicodinep,
				pdenome as nome,
				'<center><input type=\"hidden\" size=\"8\" id=\"vs_' || curid || '_' || pcfid || '_' || pdeid || '\" name=\"vagas_demanda_social_solicitadas[' || curid || '][' || pcfid || '][' || pdeid || ']\" value=\"' || COALESCE(pridemsocvagassolicitadas::text,'') || '\">' || COALESCE(pridemsocvagassolicitadas,0) || '</center>' as pridemsocvagassolicitadas,
				CASE WHEN pridemsocvagassolicitadas > 0
					THEN '<center><input type=\"text\" size=\"8\" id=\"' || curid || '_' || pcfid || '_' || pdeid || '\" name=\"vagas_demanda_social_previstas[' || curid || '][' || pcfid || '][' || pdeid || ']\" value=\"' || COALESCE(pridemvagasprevistas::text,'') || '\" onblur=\"MouseBlur(this);\" onmouseout=\"MouseOut(this);\" onfocus=\"MouseClick(this);this.select();\" class=\"normal\" onmouseover=\"MouseOver(this);\" onkeyup=\"this.value=mascaraglobal(\'[#]\',this.value);\" /></center>'
					ELSE
						'0'
				END as vagas,
				pri.estuf,
				mundescricao,
				pdilocalizacao
			from
				snf.prioridadecursoescola pri
			left join
				snf.prioridadedocumento prd ON prd.prdid = pri.prdid
			inner join
				territorios.municipio mun ON mun.muncod = pri.muncod
			where
				pcfid = $pcfid
			and
				pristatus = 'A'
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			order by
				$OrderBy";
	//dbg($sql,1);
	$arrDados = $db->carregar($sql);
	if($arrDados){
		$sql = "select pcfano from snf.prioridadecursoescola where pcfid = $pcfid limit 1";
		$pcfano = $db->pegaUm($sql);
		echo "<fieldset class=\"field_ano\" ><legend>$pcfano</legend>";
		$db->monta_lista_simples($arrDados,$arrCabecalho,1000000,10000,"N","100%","N");
		echo "</fieldset>";

		/*
		$sql = "select
					modid as codigo,
					moddesc as descricao
				from
					catalogocurso.modalidadecurso
				where
					modstatus = 'A'
				order by
					moddesc";

		$rsModalidade = $db->carregar($sql);
		$rsModalidade = $rsModalidade ? $rsModalidade : array();

		$htmlModalidade = '<select name="modid['.$curid.'][]" class="CampoEstilo" style="width: auto"><option value="">Selecione...</option>';
		foreach($rsModalidade as $dados){
			$htmlModalidade .= '<option value="'.$dados['codigo'].'">'.$dados['descricao'].'</option>';
		}
		$htmlModalidade .= '</select>';
		*/

		$sql = "select
					padid as codigo,
					paddesc as descricao
				from
					catalogocurso.publicoalvodemandasocial
				order by
					paddesc";

		$rsPublicoAlvo = $db->carregar($sql);
		$rsPublicoAlvo = $rsPublicoAlvo ? $rsPublicoAlvo : array();

		$htmlPublicoAlvo = '<select name="pdaid['.$curid.'][]" class="CampoEstilo" style="width:150px;"><option value="">Selecione...</option>';
		foreach($rsPublicoAlvo as $dados){
			$htmlPublicoAlvo .= '<option value="'.$dados['codigo'].'">'.$dados['descricao'].'</option>';
		}
		$htmlPublicoAlvo .= '</select>';

		// Inserção de vagas
		echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="tb_vagas_curso_'.$curid.'" style="display:none;">';
		echo '<thead><tr><td>Ação</td><td>CPF</td><td>Nome</td><td>Publico Alvo</td><td>E-mail</td><td colspan="2">Telefone</td><td>Celular</td></tr></thead>';
		echo '<tbody>';

		$sqlPriid = "select priid from snf.prioridadecursoescola where curid = $curid and pcfid = $pcfid;";
		$priid = $db->pegaUm($sqlPriid);

		$sql = "select * from snf.candidatodemandasocial where priid = ".$priid;
		$rsCadidatos = $db->carregar($sql);
		$rsCadidatos = $rsCadidatos ? $rsCadidatos : array();

		if($rsCadidatos){
			foreach($rsCadidatos as $i => $dados){
				echo '<tr><td><img src="/imagens/excluir.gif" style="cursor:pointer" id="'.$curid.'" onclick="excluirVaga(this)" /></td>';
				echo '<td><input type="text" id="'.$curid.'" class="normal classcpf" name="cdscpf['.$curid.'][]" value="'.formatar_cpf($dados['cdscpf']).'" onChange="verificaCPFDuplicado(this,'.$curid.')" onKeyUp="this.value=mascaraglobal(\'###.###.###-##\',this.value);" /></td>';
				echo '<td><label>'.$dados['cdsnome'].'</label><input type="hidden" class="normal" name="cdsnome['.$curid.'][]" value="'.$dados['cdsnome'].'"/></td>';
				echo '<td>'.str_replace('value="'.$dados['pdaid'].'"','value="'.$dados['pdaid'].'" selected="selected"',$htmlPublicoAlvo).'</td>';
				//echo '<td>'.str_replace('value="'.$dados['modid'].'"','value="'.$dados['modid'].'" selected="selected"',$htmlModalidade).'</td>';
				echo '<td><input type="text" class="normal" name="cdsemail['.$curid.'][]" value="'.$dados['cdsemail'].'" /></td>';
				echo '<td><input type="text" class="normal" name="cdsdddtelefonefixo['.$curid.'][]" maxlength="2" value="'.$dados['cdsdddtelefonefixo'].'" size="2"/></td>';
				echo '<td><input type="text" class="normal" name="cdstelefonefixo['.$curid.'][]" size="12" maxlength="8" value="'.$dados['cdstelefonefixo'].'" /></td>';
				echo '<td><input type="text" class="normal" name="cdstelefonecelular['.$curid.'][]" size="12" maxlength="8" value="'.$dados['cdstelefonecelular'].'" /></td></tr>';
			}
		}else{
			echo '<tr><td><img src="/imagens/excluir.gif" style="cursor:pointer" id="'.$curid.'" onclick="excluirVaga(this)" /></td>';
			echo '<td><input type="text" id="'.$curid.'_0" class="normal classcpf" name="cdscpf['.$curid.'][]" onChange="verificaCPFDuplicado(this,'.$curid.')" onKeyUp="this.value=mascaraglobal(\'###.###.###-##\',this.value);" /></td>';
			echo '<td><label></label><input type="hidden" class="normal" name="cdsnome['.$curid.'][]" /></td>';
			echo '<td>'.$htmlPublicoAlvo.'</td>';
			//echo '<td>'.$htmlModalidade.'</td>';
			echo '<td><input type="text" class="normal" name="cdsemail['.$curid.'][]" /></td>';
			echo '<td><input type="text" class="normal" name="cdsdddtelefonefixo['.$curid.'][]" size="2" maxlength="2"/></td>';
			echo '<td><input type="text" class="normal" name="cdstelefonefixo['.$curid.'][]" size="12" maxlength="8" /></td>';
			echo '<td><input type="text" class="normal" name="cdstelefonecelular['.$curid.'][]" size="12" maxlength="8" /></td></tr>';
		}
		echo '</tbody>';

		$sql = "select
				sum(COALESCE(pridemvagasprevistas,0)) as total
			from
				snf.prioridadecursoescola pri
			left join
				snf.prioridadedocumento prd ON prd.prdid = pri.prdid
			inner join
				territorios.municipio mun ON mun.muncod = pri.muncod
			where
				pcfid = $pcfid
			and
				pristatus = 'A'
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");

		$totalVagas = $db->pegaUm($sql);
		$inputVagas = '<input type="hidden" name="vagas_informadas[]" id="vagas_informadas_'.$curid.'" value="'.$totalVagas.'" /><input type="hidden" name="saldo_vagas[]" id="saldo_vagas_'.$curid.'" value="" />';

		echo '<tfoot><tr><td colspan="9"><a href="javascript:void(0)" onclick="inserirVaga(\''.$curid.'\')"><hr/><img border=\"0\" src="/imagens/gif_inclui.gif" class="img_middle"/>&nbsp;Inserir vaga</a>'.$inputVagas.'</td></tr></tfoot>';
		echo '</table>';


	}
}


function salvarPrioridadeCursos(){
	global $db;

	extract($_POST);
	
	if($prioridade){
		foreach($prioridade as $curid => $arrAnos){
			if($arrAnos){
				foreach($arrAnos as $pcfid => $arrEscolas){
					foreach($arrEscolas as $pdeid => $arrModid){
						foreach ($arrModid as $modid => $priordem){
							if($arrEscolas){
								//$privagasprevistas = $vagas_previstas[$curid][$pcfid][$pdeid] || $vagas_previstas[$curid][$pcfid][$pdeid] === 0 ? $vagas_previstas[$curid][$pcfid][$pdeid] : "null";
								$privagasprevistas = $vagas_previstas[$curid][$pcfid][$pdeid][$modid] || $vagas_previstas[$curid][$pcfid][$pdeid][$modid] === 0 ? $vagas_previstas[$curid][$pcfid][$pdeid][$modid] : 0;
								if($curid != '' && $pdeid != '' && $pcfid != '' && $pcfano[$curid][$pcfid][$pdeid][$modid] != ''){
									$sql = "
										update snf.prioridadecursoescola set
											priordem = $priordem,
											privagasprevistas = ".( $privagasprevistas )."
										where curid = $curid and pdeid = $pdeid and pcfid = $pcfid and pcfano = {$pcfano[$curid][$pcfid][$pdeid][$modid]} and modid = $modid;
									";
									if($sql){
										$db->executar($sql);
										$db->commit();
									}
								}
							}
						}
					}
				}
			}
		}
	}
	die("ok");
}

function excluirEscola()
{
	global $db;

	$priid = $_POST['priid'];

	if($priid){
		$sql = "update snf.prioridadecursoescola set pristatus = 'I' where priid = $priid";
		$db->executar($sql);
		$db->commit();
	}

	die("ok");
}

function excluirCurso()
{
	global $db;

	$curid = $_POST['curid'];
	$modid = $_POST['modid'];
	$pcfid = $_POST['pcfid'];
	$ncuid = $_POST['ncuid'];

	$arrWhere[] = "estuf = '{$_POST['estuf']}'";

	if($_POST['muncod']){
		$arrWhere[] = "muncod = '{$_POST['muncod']}'";
		$arrWhere[] = "pdiesfera = 'Municipal'";
	}else{
		$arrWhere[] = "pdiesfera = 'Estadual'";
	}

	if($curid){
		$sql = "update
					snf.prioridadecursoescola
				set
					pristatus = 'I'
				where
					curid = $curid
				and
					modid = $modid
				and
					pcfid = $pcfid
				and
					ncuid = $ncuid
				".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");
		$db->executar($sql);
		$db->commit();
	}
	die("ok");
}

function salvarTipoOdemCurso($ano = null, $ordem = null){
	global $db;
	extract($_POST);
	
	$modid = $_POST['modid'];
	$ncuid = $_POST['ncuid'];
	$curid = $_POST['curid'];
	$pcfid = $ano;
	$pcfid = !$pcfid ? $_POST['pcfid'] : $pcfid;

	$arrWhere[] = "pri.estuf = '$estuf'";

	if($muncod){
		$arrWhere[] = "pri.muncod = '$muncod'";
		$arrWhere[] = "pdiesfera = 'Municipal'";
	}else{
		$arrWhere[] = "pdiesfera = 'Estadual'";
	}

	if($_POST['ateid']){
		$arrWhere[] = "ateid = '{$_POST['ateid']}'";
	}
	if($_POST['curid']){
		$arrWhere[] = "curid = '{$_POST['curid']}'";
	}
	if($modid){
		$arrWhere[] = "modid = '{$modid}'";
	}
	if($ncuid){
		$arrWhere[] = "ncuid = '{$ncuid}'";
	}

	switch($ordem){
		case "idebai":
			$prddescordem = "I";
			break;
		case "idebaf":
			$prddescordem = "F";
			break;
		default:
			$prddescordem = "P";
	}

	$sql = "
		Select prd.prdid
		From snf.prioridadedocumento prd
		inner join snf.prioridadecursoescola pri ON pri.prdid = prd.prdid
		Where pcfid = $pcfid and pristatus = 'A'
		".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");

	$prdid = $db->pegaUm($sql);

	if($prdid){
		$sql = "update snf.prioridadedocumento set prddescordem = '$prddescordem' where prdid = $prdid";
		$db->executar($sql);
		$db->commit();
	}
}

function verificaPreenchimentoVagas( $estuf, $muncod = null, $pcfid, $docid_usado )
{
	global $db;

	if(!$pcfid){
		return false;
	}
	if(!$docid_usado){
		return false;
	}

	$arrWhere[] = "estuf = '$estuf'";

	$arrWhere[] = "pcfid in($pcfid)";

	if($muncod){
		$arrWhere[] = "muncod = '$muncod'";
		$arrWhere[] = "pdiesfera = 'Municipal'";
	}else{
		$arrWhere[] = "pdiesfera = 'Estadual'";
	}

	$sql = "select
				count(priid)
			from
				snf.prioridadecursoescola pri
			inner join
				snf.prioridadedocumento doc ON doc.prdid = pri.prdid
			where
				privagasprevistas is null
			and
				pristatus = 'A'
			and
				".(implode(" and ",$arrWhere));
	$num_cursos = $db->pegaUm($sql);

	$sql = "select
				count(priid)
			from
				snf.prioridadecursoescola pri
			inner join
				snf.prioridadedocumento doc ON doc.prdid = pri.prdid
			where
				pridemvagasprevistas is null
			and
				pristatus = 'A'
			and
				pridemsoc = 'S'
			and
				".(implode(" and ",$arrWhere));
	$num_demanda_social = $db->pegaUm($sql);

	if($num_cursos){
		return "Existe(m) $num_cursos curso(s) com Plano Autorizado não preenchido.";
	}elseif($num_demanda_social){
		return "Existe(m) $num_demanda_social curso(s) com Demanda Social não preenchida.";
	}else{
		return true;
	}
}

function tramitaTodosDocumentos($estuf, $muncod = null, $pcfid, $docid_usado)
{
	global $db;

	$arrWhere[] = "estuf = '$estuf'";

	$arrWhere[] = "pcfid in($pcfid)";

	if($muncod){
		$arrWhere[] = "muncod = '$muncod'";
		$arrWhere[] = "pdiesfera = 'Municipal'";
	}else{
		$arrWhere[] = "pdiesfera = 'Estadual'";
	}

	$sql = "select
				docid
			from
				snf.prioridadecursoescola pri
			inner join
				snf.prioridadedocumento doc ON doc.prdid = pri.prdid
			where
				privagasprevistas is not null
			and
				pristatus = 'A'
			and
				docid != $docid_usado
			and
				".(implode(" and ",$arrWhere));
	$arrDocid = $db->carregarColuna($sql);
	$arrDados =array("estuf" => $estuf,"muncod" => $muncod,"pcfid" => $pcfid, "docid_usado" => $docid_usado);
	foreach($arrDocid as $docid){

		$aedid = WORKFLOW_SNF_DEFINIR_PRIORIDADE;
		$acao = wf_pegarAcao2( $aedid );
		$esdiddestino = (integer) $acao['esdiddestino'];

		// cria log no histórico
		$sqlHistorico = "
			insert into workflow.historicodocumento
			( aedid, docid, usucpf, htddata )
			values ( " . $aedid . ", " . $docid . ", '" . $_SESSION['usucpf'] . "', now() )
			returning hstid
		";
		$hstid = (integer) $db->pegaUm( $sqlHistorico );

		// cria comentário
		$sqlComentario = "
			insert into workflow.comentariodocumento
			( docid, hstid, cmddsc, cmddata, cmdstatus )
			values ( " . $docid . ", " . $hstid . ", 'Prioridade Definida', now(), 'A' )
		";
		$db->executar( $sqlComentario);

		// atualiza documento
		$sqlDocumento = "
			update workflow.documento
			set esdid = " . $esdiddestino . "
			where docid = " . $docid;

		$db->executar( $sqlDocumento );

		$db->commit();
	}
	return true;

}


function devolverTodosDocumentos($estuf, $muncod = null, $pcfid, $docid_usado)
{
	global $db;

	$arrWhere[] = "estuf = '$estuf'";

	$arrWhere[] = "pcfid in($pcfid)";

	if($muncod){
		$arrWhere[] = "muncod = '$muncod'";
		$arrWhere[] = "pdiesfera = 'Municipal'";
	}else{
		$arrWhere[] = "pdiesfera = 'Estadual'";
	}

	$sql = "select
				docid
			from
				snf.prioridadecursoescola pri
			inner join
				snf.prioridadedocumento doc ON doc.prdid = pri.prdid
			where
				privagasprevistas is not null
			and
				pristatus = 'A'
			and
				docid != $docid_usado
			and
				".(implode(" and ",$arrWhere));
	$arrDocid = $db->carregarColuna($sql);
	$arrDados =array("estuf" => $estuf,"muncod" => $muncod,"pcfid" => $pcfid, "docid_usado" => $docid_usado);
	foreach($arrDocid as $docid){

		$aedid = WORKFLOW_SNF_DEVOLVER_ANALISE_PRIORIDADE;
		$acao = wf_pegarAcao2( $aedid );
		$esdiddestino = (integer) $acao['esdiddestino'];

		// cria log no histórico
		$sqlHistorico = "
			insert into workflow.historicodocumento
			( aedid, docid, usucpf, htddata )
			values ( " . $aedid . ", " . $docid . ", '" . $_SESSION['usucpf'] . "', now() )
			returning hstid
		";
		$hstid = (integer) $db->pegaUm( $sqlHistorico );

		// cria comentário
		$sqlComentario = "
			insert into workflow.comentariodocumento
			( docid, hstid, cmddsc, cmddata, cmdstatus )
			values ( " . $docid . ", " . $hstid . ", 'Devolver para Análise - Refazer Priorização', now(), 'A' )
		";
		$db->executar( $sqlComentario);

		// atualiza documento
		$sqlDocumento = "
			update workflow.documento
			set esdid = " . $esdiddestino . "
			where docid = " . $docid;

		$db->executar( $sqlDocumento );

		$db->commit();
	}
	return true;

}

function listaAdesoes()
{
	global $db;

	if($_POST){
		extract($_POST);

		if($inscnpj){
			$inscnpj = str_replace(array(".","/","-"),array("","",""),$inscnpj);
			$arrWhere[] = "inscnpj ilike('%$inscnpj%')";
		}
		if($insnome){
			$arrWhere[] = "insnome ilike('%$insnome%')";
		}
		if($inssigla){
			$arrWhere[] = "inssigla ilike('%$inssigla%')";
		}
		if($estuf){
			$arrWhere[] = "mun.estuf = '$estuf'";
		}
		if($muncod){
			$arrWhere[] = "mun.muncod = '$muncod'";
		}
		if($dirnome){
			$arrWhere[] = "dir.dirnome ilike('%$dirnome%')";
		}
		if($dircpf){
			$dircpf = str_replace(array(".","/","-"),array("","",""),$dircpf);
			$arrWhere[] = "dircpf = '$dircpf' ";
		}
		if($teraprovacao){
			$arrWhere[] = "teraprovacao = '$teraprovacao'";
		}
	}

	$sql = "select 
			'<img src=\"../imagens/consultar.gif\" title=\"Visualizar Dados\" class=\"link img_middle\" onclick=\"visualizarAdesao(' || ins.insid || ')\"  />' as acao,
			ins.insnome, 
			dir.dirnome, 
			dir.dircpf, 
			to_char(ter.terdatahora,'DD/MM/YYYY'), 
			mem.memnome, 
			mem.memcpf, 
			mem.mememail 
			from
				snf.instituicaoensino ins
			inner join
				snf.naturezajuridica nat ON nat.natid = ins.natid
			inner join
				snf.endereco ende ON ende.insid = ins.insid
			inner join
				territorios.municipio mun ON mun.muncod = ende.muncod
			inner join
				snf.termoadesao ter ON ter.insid = ins.insid
			inner join
				snf.dirigentemaximo dir ON dir.insid = ins.insid
			inner join 
				snf.comite com ON ins.insid = com.insid
			inner join 
				snf.membrocomite mem ON mem.comid = com.comid
				
			where ter.aneid is not null
			and
				insstatus = 'A'
			and
				endstatus = 'A'
			and
				dir.dirstatus = 'A'
			and 
				mem.papid = 9	
			".($arrWhere ? " and ". implode(" and ",$arrWhere) : "")."
			order by
				ins.insnome,mun.estuf,mun.mundescricao";


	$arrCab = array("Ação","Instituição", "Dirigente", "CPF Dirigente", "Adesão","Coordenador Institucional","CPF Coordenador","E-mail Coordenador");
	$db->monta_lista($sql,$arrCab,100,10,"N","center","N");
}

function pegaDadosInstituicaoPorId($insid)
{
	global $db;

	$sql = "select
				dir.dirnome,
				dir.dircpf,
				dir.dirid,
				dir.carid,
				arq.arqid,
				arq.arqnome || '.' || arq.arqextensao as anexo_dirigente,
				ins.inscnpj,
				ins.insnome,
				ins.inssigla,
				ins.instelefone,
				ins.insfax,
				ins.insemail,
				ende.*,
				mun.mundescricao,
				mun.estuf,
				ins.insid
			from
				snf.dirigentemaximo dir
			--inner join
				--seguranca.usuario usu ON usu.usucpf = dir.dircpf
			left join
				snf.anexo ane ON ane.aneid = dir.aneid and ane.anestatus = 'A'
			left join
				public.arquivo arq ON arq.arqid = ane.arqid
			inner join
				snf.instituicaoensino ins ON ins.insid = dir.insid
			left join
				snf.endereco ende on ende.insid = ins.insid and ende.endstatus = 'A'
			/*left join
				entidade.endereco ende2 on ende2.entid = ins.entid --and ende2.endstatus = 'A'*/
			left join
				territorios.municipio mun on mun.muncod = ende.muncod
			where
				ins.insid = '$insid'
			and
				dir.dirstatus = 'A'
			and
				ins.insstatus = 'A'";
	//dbg($sql);
	return $db->pegaLinha($sql);

}

function aprovarAdesao()
{
	global $db;

	$insid = $_POST['insid'];

	$sql = "update
				snf.termoadesao
			set
				teraprovacao = 'S'
			where
				insid = $insid;";

	$db->executar($sql);
	$db->commit();
	enviaEmailDirigente($insid);
	$db->sucesso("principal/listaAdesoes","");
	exit;
}

function rejeitarAdesao()
{
	global $db;

	$insid = $_POST['insid'];

	$sql = "update snf.anexo set anestatus = 'I' where aneid = (select aneid from snf.termoadesao where insid = $insid);
			delete from
				snf.termoadesao
			where
				insid = $insid;";

	$db->executar($sql);
	$db->commit();
	$db->sucesso("principal/listaAdesoes","");
	exit;
}

function enviaEmailDirigente($insid)
{
	global $db;

	$sql = "select
				insnome,
				inscnpj
			from
				snf.instituicaoensino
			where
				insid = $insid";
	$arrDados = $db->pegaLinha($sql);

	$sql = "select distinct
				usuemail
			from
				seguranca.usuario usu
			inner join
				snf.dirigentemaximo ins ON ins.dircpf = usu.usucpf
			and
				dirstatus = 'A'
			and
				ins.insid = $insid";

	$arrUsu = $db->carregarColuna($sql);

	$arrUsu[] = "julianomeinen.souza@gmail.com";

	if($arrUsu && !strstr($_SERVER['SERVER_NAME'],"simec-local")){
		include APPRAIZ . 'includes/classes/EmailAgendado.class.inc';
		$e = new EmailAgendado();
		$e->setTitle("Programa de Formação Continuada");
		$html = 'Senhor(a) Dirigente(a),<br /><br />
		 			A instituição de ensino '.$arrDados['insnome'].', inscrita sob o CNPJ '.mascaraglobalTermoAdesao($arrDados['inscnpj'],"##.###.###/####-##").' teve a adesão aprovada.<br />
		 			Favor Verificar.';
		$e->setText($html);
		$e->setName("Aprovação - Programa de Formação Continuada");
		$e->setEmailOrigem("no-reply@mec.gov.br");
		$e->setEmailsDestino($arrUsu);
		$e->enviarEmails();
	}
}


function salvarDemandaSocial()
{
	global $db;

	extract($_POST);

	if($vagas_demanda_social_previstas){
		foreach($vagas_demanda_social_previstas as $curid => $arrAnos){
			if($arrAnos){
				foreach($arrAnos as $pcfid => $arrEscolas){
					foreach($arrEscolas as $pdeid => $demanda){
						if($arrEscolas){
							$demanda = !$demanda ? 0 : $demanda;
							$sql.= "UPDATE snf.prioridadecursoescola
									   SET pridemvagasprevistas = $demanda
									 WHERE curid = $curid
									   AND pdeid = $pdeid
									   AND pcfid = $pcfid;";

							if(count($cdscpf[$curid]) > 0){
								$sqlPriid = "SELECT priid
											   FROM snf.prioridadecursoescola
											  WHERE	curid = $curid
												AND	pdeid = $pdeid
												AND	pcfid = $pcfid;";

								$priid = $db->pegaUm($sqlPriid);

								$sqlDel = "DELETE FROM snf.candidatodemandasocial
												 WHERE priid = ".$priid;
								$db->executar($sqlDel);

								foreach($cdscpf[$curid] as $k => $value){
									$value = str_replace(array('.','-'), '',$value);
									$sql .= "INSERT INTO snf.candidatodemandasocial(cdscpf,
																					priid,
																					pdaid,
																					--modid,
																					cdsnome,
																					cdsemail,
																					cdsdddtelefonefixo,
																					cdstelefonefixo,
																					cdstelefonecelular,
																					cdsstatus)
											 	VALUES								('{$value}',
																					{$priid},
																					".($pdaid[$curid][$k] ? $pdaid[$curid][$k] : 'null').",
																					--'{$modid[$curid][$k]}',
																					'{$cdsnome[$curid][$k]}',
																					'{$cdsemail[$curid][$k]}',
																					'{$cdsdddtelefonefixo[$curid][$k]}',
																					'{$cdstelefonefixo[$curid][$k]}',
																					'{$cdstelefonecelular[$curid][$k]}',
																					'A');";
								}
							}
						}
					}
				}
			}
		}
	}

	if($sql){
		$db->executar($sql);
		$db->commit();
	}
	die("ok");
}

function verica_cadastro(){

	global $db;
	return true;
}

function enviarcurso($request){
	global $db;

	$estuf  = $_SESSION['snf']['est_uf'];
	$muncod = $_SESSION['snf']['mun_cod'];
	$esfera = $_SESSION['snf']['esfera'];
	
	$sql_esc = "
		update snf.sugestaocurso Set sucsugescolha = 'N' 
		where pdiesfera = '$esfera' and estuf = '$estuf' and muncod = '$muncod'
	";
	$db->executar($sql_esc);
	
	$sql = "
		Insert Into snf.sugestaocurso(
				pdiesfera,
				estuf,
				muncod,
				foccurso,
				focdescricao,
				focjustificativa,
				sucsugsec,
				sucsugescolha,
				usucpf,
				sucstatus
		)Values(
				'$esfera',
				'$estuf',
				'$muncod',
				'".$request['foccurso']."',
				'".$request['focdescricao']."',
				'".$request['focjustificativa']."',
				'S',
				'S',
				'".$_SESSION['usucpf']."',
				'A')Returning sucid
	";
	$sucid = $db->pegaUm($sql);
	$db->commit($sql);

	$sql = '';

	if( $request['fexid'][0] != '' ){
		foreach( $request['fexid'] as $id ){
			$sql .= "Insert Into snf.sugestaocursofuncao(sucid, fexid)Values($sucid, $id);";
		}
	}

	if( $request['pk_cod_etapa_ensino'][0] != '' ){
		foreach( $request['pk_cod_etapa_ensino'] as $pk_cod_etapa_ensino ){
			$sql .= "Insert Into snf.sugestaocursoetapaensino (sucid, pk_cod_etapa_ensino)Values($sucid, $pk_cod_etapa_ensino);";
		}
	}

	if( $request['pk_cod_disciplina'][0] != '' ){
		foreach( $request['pk_cod_disciplina'] as $pk_cod_disciplina ){
			$sql .= "Insert Into snf.sugestaocursodisciplina (sucid, pk_cod_disciplina)Values($sucid, $pk_cod_disciplina);";
		}
	}

	if( $request['pk_cod_mod_ensino'][0] != '' ){
		foreach( $request['pk_cod_mod_ensino'] as $pk_cod_mod_ensino ){
			$sql .= "Insert Into snf.sugestaocursomodalidade (sucid, pk_cod_mod_ensino)Values($sucid,$pk_cod_mod_ensino);";
		}
	}

	$db->executar($sql);
	$db->commit();
	//$db->rollback();
	echo "<script>alert('Dados salvos.');window.location='snf.php?modulo=principal/sugestaoCurso&acao=A';</script>";

}


function set_curso_sugestao($request){
	global $db;
	
	$sucid = $_REQUEST['sucid'];
	
	$estuf  = $_SESSION['snf']['est_uf'];
	$muncod = $_SESSION['snf']['mun_cod'];
	$esfera = $_SESSION['snf']['esfera'];
	
	if(!$estuf || !$muncod){
		$cpfusu = $_SESSION['usucpf'];
		$arryRecupera = recuperaEstadoMunicipioPerfil($cpfusu);
		$estuf  = $arryRecupera['estuf'];
		$muncod = $arryRecupera['muncod'];
	}
	
	$sql_esc = "
		update snf.sugestaocurso Set sucsugescolha = 'N' 
		where pdiesfera = '$esfera' and estuf = '$estuf' and muncod = '$muncod'
	";
	$db->executar($sql_esc);
	
	$sql_up = "
		update snf.sugestaocurso Set sucsugescolha = 'S' 
		where sucid = $sucid
	";

	$db->executar($sql_up);
	$db->commit();

	echo "OK";

}


function dadosCursoPopup($curid)
{
	global $db;
	$sql = "select
				curdesc,
				curobjetivo,
				curmetodologia,
				curcertificado,
				curementa,
				curfunteome,
				curinfra
			from
				catalogocurso.curso dmd
			where
				curid = $curid";
	$arrCab = array("Curso","Objetivo","Metodologia","Certificado","Ementa","Fundamentos Teóricos","Infraestrutura");
	$db->monta_lista_simples($sql,$arrCab,10,10);
	/*
	$arrDados = $db->pegaLinha($sql);
	$n = 0;
	if($arrDados){
		foreach($arrDados as $dado){
			if((strlen($dado) > 150)){
				//$nova_string = substr($dado,0,150); //pega só os primeiros 200 caracteres
				$nova_string = strrev($nova_string); //inverte a string
				$pos_ini = strpos($nova_string," ");  //pega o uútimo espaço em branco
				if($pos_ini){
					$pos_fim = strlen($dado);
					$nova_string = substr($nova_string,$pos_ini,$pos_fim);
					$dado = strrev($nova_string);
				}
				$dado.= "...";
			}
			echo "<b>{$arrCab[$n]}</b>: ".$dado."<br />";
			$n++;
		}
	}else{
		echo "<b>Não existem informações.</b>";
	}
	*/

	exit;
}

function cabecalhoListaEscolas($estuf, $muncod = null, $esfera){
	global $db;
	
	echo '<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">';
	echo '<tr>';
	echo '	<td class="subtituloDireita" width="220">Orientações</td>';
	echo '		<td>';
	echo '			Aqui você deve abrir cada um dos cursos e realizar uma priorização entre as escolas. Caso não haja vagas disponíveis para todos os professores, a priorização de escolas vai determinar os professores que poderão participar dos cursos, na ordem de prioridade estabelecida por cada escola.';
	echo '			<p>A tela já é apresentada com uma priorização pelo IDEB dos anos iniciais, com a escola de menor IDEB sendo a primeira prioridade. Esta priorização é apenas uma sugestão e pode ser  alterada modificando os números da ordem de prioridade e criando uma priorização individualizada pela Secretaria.</p>';
	echo '			Este ano, só é possível modificar , priorizar e validar o período 2012/2013.';
	echo '		</td>';
	echo '</tr>';

	$sql = "Select estdescricao From territorios.estado Where estuf = '$estuf'";
	$rsEstado = $db->pegaUm($sql);

	if($rsEstado != ''){
		echo '<tr><td class="subtituloDireita" width="220">Estado</td><td>'.$rsEstado.' - '.$estuf.'</td></tr>';
	}

	$stWhere = '';
	if($muncod){
		$stWhere .= " and pde.muncod = '$muncod' ";
		
		$sql = "Select mundescricao From territorios.municipio Where muncod = '{$muncod}'";
		$rsMunicipio = $db->pegaUm($sql);

		echo '<tr><td class="subtituloDireita">Município</td><td>'.$rsMunicipio.'</td></tr>';
	}

	$sql = "
		Select	floor(count(distinct d)*0.2) as vagas
		From educacenso_2010.tab_docente d
		Inner Join educacenso_2010.tab_dado_docencia ee ON d.pk_cod_docente = ee.fk_cod_docente
		Join (Select cast(pdicodinep as integer) as pdicodinep, estuf, muncod, pdiesfera
		      From dblink('host= user= password= port= dbname=',
		      'Select cast(pdicodinep as integer) as pdicodinep, estuf, muncod, pdiesfera
		      From pdeinterativo.pdinterativo
		      ') as (pdicodinep integer, estuf varchar, muncod varchar, pdiesfera varchar)
		) as pde on pde.pdicodinep = ee.fk_cod_entidade
		Where pde.estuf = '$estuf' and pde.pdiesfera = '$esfera'		
		$stWhere
	";		
//die($sql);
	$vagasDisponiveis = $db->pegaUm($sql);
	$_SESSION['vagasDisponiveis'] = $vagasDisponiveis;
	
	if($vagasDisponiveis){
		echo '<tr><td class="subtituloDireita">Vagas</td><td>'.$vagasDisponiveis.'</td></tr>';
	}

	
	$sql = "
		Select sum(privagasprevistas) as vagas
		From snf.prioridadecursoescola
		where estuf = '{$estuf}' and pdiesfera = ".($muncod ? "'Municipal'" : "'Estadual'")." and pristatus = 'A' and pcfid in (1,2)
		".str_replace('pde.', '', $stWhere);
//die($sql);
	$vagasSolicitadas = $db->pegaUm($sql);
	$_SESSION['vagasSolicitadas'] = $vagasSolicitadas;

	if($vagasDisponiveis < $vagasSolicitadas){
		echo '<tr>';
		echo '<td class="subtituloDireita">Atenção</td><td><img src="../imagens/atencao.png" align="absmiddle" />';
		echo '<font color="red">O número de vagas disponíveis ('.$vagasDisponiveis.') é menor que as vagas autorizadas ('.$vagasSolicitadas.')!</font>';
		echo '</td>';
		echo '</tr>';
	}
	echo '</table>';
}


function carregaPerfil(){
	global $db;
	
	$usucpf = $_SESSION['usucpf'];
	
	$sql = "
		Select 	p.pflcod as pflcod
		From seguranca.perfil as p
		Join seguranca.perfilusuario as pu on pu.pflcod = p.pflcod
		Where pu.usucpf = '$usucpf'
	";
	$perfil_cod = $db->carregar($sql);

	foreach($perfil_cod as $dados){
		$arrPerfil[] = $dados['pflcod'];
	}

	return $arrPerfil;
}

function regrasGrupoWorkflow(){
	global $db;

	$id_estaDocumeto = WORKFLOW_SNF_EM_VALIDACAO_PELA_SECRETARIA;
	$id_tipoDocumeto = WORKFLOW_SNF;
	
	$sql = "
		Select	muncod, 
				estuf 
		From snf.usuarioresponsabilidade 
		where usucpf = '{$_SESSION['usucpf']}'
	";
	$esferaPerfilResponsavel = $db->pegaLinha($sql);

	if(checkPerfil(array(PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PERFIL_EQUIPE_MUNICIPAL)) && $esferaPerfilResponsavel['muncod']){
		$where = "Where muncod = '".$esferaPerfilResponsavel['muncod']."'";
		$esfera_mun = $esferaPerfilResponsavel['muncod'];
		$priWhere = "Where muncod = '".$esferaPerfilResponsavel['muncod']."' and pdiesfera = 'Municipal'";
	}elseif(checkPerfil(array(PERFIL_EQUIPE_ESTADUAL_APROVACAO, PERFIL_EQUIPE_ESTADUAL)) && $esferaPerfilResponsavel['estuf']){
		$where = "Where estuf = '".$esferaPerfilResponsavel['estuf']."'";
		$esfera_uf = $esferaPerfilResponsavel['estuf'];
		$priWhere = "Where estuf = '".$esferaPerfilResponsavel['estuf']."' and pdiesfera = 'Estadual'";
	}
	
	$sql = "
		Select prdid from snf.prioridadedocumento
		$where
	";
	$priDoc = $db->pegaLinha($sql);
	
	if($priDoc['prdid'] == ''){
		$slq_work = 
			"Insert into workflow.documento (
				tpdid, 
				esdid, 
				docdsc, 
				docdatainclusao
			)Values(
				$id_tipoDocumeto,
				$id_estaDocumeto,
				'Em Análise (Secretaria de Educação)',
				'now()'
			)Returning docid;
		";
		$docid = $db->pegaUm($slq_work);
		$db->commit();
	
		if($esfera_mun){
			//--Se Municipal
			$sql_insert = "Insert Into snf.prioridadedocumento (docid, muncod) VALUES ($docid, '$esfera_mun') returning prdid;";
		}elseif($esfera_uf){
			//--Se Estadual
			$sql_insert = "Insert Into snf.prioridadedocumento (docid, estuf) VALUES ($docid, '$esfera_uf') returning prdid;";
		}
		$prdid = $db->pegaUm($sql_insert);
		$db->commit();
#A pedido do Cid na data 24/10/2012, essa parte do código foi comentado, para a correção de um problema. 
#De acordo com o analista apenas o comentário do dódigo é o suficiente para solucionar o problma, não sendo necessário nenhuma outra medida!   
// 		$sql_up = "
// 			Update snf.prioridadecursoescola
// 			set prdid = $prdid
// 			$priWhere
// 		";
// 		$db->executar($sql_up);
// 		$db->commit();
	}
// 	else{
// 		$sql_up = "
// 			Update snf.prioridadecursoescola
// 			set prdid = ".$priDoc['prdid']."
// 			$priWhere
// 		";
// 		$db->executar($sql_up);
// 		$db->commit();
// 	}
}

function pegaDocid($muncod, $estuf){
	global $db;

	if($muncod){
		$where = "Where muncod = '$muncod'";
	}else{
		$where = "Where estuf = '$estuf'";
	}

	$sql = "
		Select	docid
		From snf.prioridadedocumento
		$where
	";
	$docid = $db->pegaUm($sql);
	return $docid;
}

function pegaEstadoAtual($docid){
	global $db;

	if($docid) {
		$docid = (integer) $docid;
		$sql = "
			Select ed.esdid
			From workflow.documento d
			inner join workflow.estadodocumento ed on ed.esdid = d.esdid
			where d.docid = $docid
		";
		$estado = $db->pegaUm($sql);
		return $estado;
	} else {
		return false;
	}
}

function pegaEstadoParaEnvio($estuf, $muncod){
	global $db;
	
	$esfera = $_SESSION['snf']['esfera'];
	if($esfera == 'Estadual'){
		$where = "where estuf = '$estuf' and pdiesfera = '$esfera'";
	}else{
		$where = "where estuf = '$estuf' and muncod = '$muncod' and pdiesfera = '$esfera'";
	}
	
	$sql = "
		Select count(case when privagasprevistas isnull then 1 end) as qdt
		From snf.prioridadecursoescola
		$where
		and pristatus = 'A' and pcfid in (1,2)
	";
	$qdt = $db->pegaUm($sql);
	
	if($qdt[qdt] == 0 && ($_SESSION['vagasDisponiveis'] >= $_SESSION['vagasSolicitadas'])) {
		return true;
	}else{
		return false;
	}
}

function enviaEmailProfessor($estuf, $muncod, $pcfid){
	global $db;
	$esfera = $_SESSION["snf"]["esfera"]; 
	if($esfera == 'Municipal'){
		$where = "WHERE muncod = '$muncod'
				 AND pdiesfera = 'Municipal'";	
	} else { 
		$where = "WHERE estuf = '$estuf'
				 AND pdiesfera = 'Estadual'"; 
	}
	
	$sql = "SELECT distinct curid, modid, ncuid, pcfid
			FROM snf.prioridadecursoescola pri
			$where
			AND pristatus = 'A' 
			AND pri.pcfid IN ($pcfid)
			GROUP BY curid, modid, ncuid, pcfano, pcfid
			ORDER BY curid"; 
	
	$aryCurso = $db->carregar($sql);
	
	foreach($aryCurso as $curso){
		$sqlv = "SELECT sum(coalesce(privagassolicitadas,0)) as sol, sum(coalesce(privagasprevistas,0)) as aut
				FROM snf.prioridadecursoescola 
				$where 
				AND curid = $curso[curid]
				AND modid = $curso[modid]
				AND ncuid = $curso[ncuid]
				AND pcfid = $curso[pcfid]";
		
		$aryVagas = $db->pegaLinha($sqlv);
		$corte = $aryVagas["sol"] - $aryVagas["aut"]; 
	
		$sqle = "SELECT pdeid
				FROM snf.prioridadecursoescola 
				$where
				AND curid = $curso[curid]
				AND modid = $curso[modid]
				AND pcfid = $curso[pcfid]				
				ORDER BY priordem, curid";

		$aryPrioridadeEscola = $db->carregar($sqle);
		$tmp = array();
		
		foreach($aryPrioridadeEscola as $escola => $esc){
    		$tmp[] = implode(",", $esc);
		}

		$esc = implode(",", $tmp); 
		
		if($aryVagas["sol"] != $aryVagas["aut"]){
			$sqlp = "SELECT pfd.pfdemail, pfd.pfdcpf, ps.pesnome, c.curdesc 
					FROM pdeinterativo.planoformacaodocente pfd
					INNER JOIN catalogocurso.curso c
		      	 	ON c.curid = pfd.curid
					INNER JOIN pdeinterativo.pdinterativo p
		            ON pfd.pdeid = p.pdeid
					LEFT JOIN pdeinterativo.pessoa ps
	  	            ON pfd.pfdcpf = ps.usucpf
					$where
					AND trim(pfd.pfdemail) <> ''
					AND pfd.curid = $curso[curid]
					AND pfd.modid = $curso[modid]
					AND c.ncuid = $curso[ncuid]
					AND pfd.pcfid = $curso[pcfid]
					AND pfd.pfdstatus = 'A'
					AND pfd.pdeid IN ($esc)
					ORDER BY pfd.pfdprioridade
					LIMIT $aryVagas[sol]
					OFFSET $aryVagas[aut]";
			
			$aryPessoa = $db->carregar($sqlp);
			if(!empty($aryPessoa)){
				foreach($aryPessoa as $pessoa){
					$remetente 	= 'no-reply@mec.gov.br'; 
					$assunto	= 'Programa de Formação Continuada';
					$conteudo	= '<p>Caro(a) Professor(a) '. $pessoa['pesnome'].',</p>
								O seu pedido para participar do curso '.$pessoa['curdesc'].', realizado pelo diretor de sua escola no PDE Interativo, não foi validado pela Secretaria da sua Rede de Ensino, 
								em decorrência do limite de vagas que o sistema permite que sejam autorizadas pela Secretaria. O planejamento da escola poderá ser revisto no começo do ano que vem e seu 
								nome pode ser incluído novamente em qualquer curso e para qualquer período. 
								<p>Atenciosamente,</p>
								<p>Sistema Nacional de Formação - SINAFOR<br>Ministério da Educação<br>0800 61 61 61 Opção 6<br>http://sinafor.mec.gov.br</p>';
					//$destinatario = $pessoa['pfdemail'];
					$cc			= array($_SESSION['email_sistema']);
					$cco		= ''; 
					$arquivos 	= array();
					enviar_email($remetente,$destinatario,$assunto, $conteudo, $cc, $cco, $arquivos );	
				}
			}
		}
	}
}

function cabecalhoInstituicao($insid=null)
{
	global $db;
	
	if ( empty($insid) ){
		$sql = "select
					ie.insid, 
					ie.entid,
					ie.insnome,
					ie.inssigla,
					ie.inscnpj,
					ie.instelefone,
					ie.insfax,
					ie.insemail 
				from 
					snf.usuarioresponsabilidade ur
				inner join 
					snf.instituicaoensino ie on ie.insid = ur.insid 
						and ie.insstatus = 'A'
				where 
					ur.usucpf = '{$_SESSION['usucpf']}'
				and
					ur.rpustatus = 'A'
				order by
					rpuid desc";
	}else{
		$sql = "select
					ie.insid, 
					ie.entid,
					ie.insnome,
					ie.inssigla,
					ie.inscnpj,
					ie.instelefone,
					ie.insfax,
					ie.insemail 
				from 
					snf.instituicaoensino ie 
				where 
					ie.insstatus = 'A' AND
					ie.insid = {$insid}";
	}
	$rs = $db->pegaLinha($sql);
	
	if($rs['insid'] && empty($_SESSION['snf']['insid'])){
		$_SESSION['snf']['insid'] = $rs['insid'];
	}
	
	if($rs){
		
		echo '<input type="hidden" name="entid" id="entid" value="'.$rs['entid'].'" />
			  <input type="hidden" name="insid_temp" id="insid_temp" value="'.$rs['insid'].'" />
			  <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
				<tr>
					<td class="subtituloDireita">Nome</td>
					<td>'.$rs['insnome'].'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Sigla</td>
					<td>'.$rs['inssigla'].'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">CNPJ</td>
					<td>'.formatar_cnpj($rs['inscnpj']).'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Telefone</td>
					<td>'.$rs['instelefone'].'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Fax</td>
					<td>'.$rs['nsfax'].'</td>
				</tr>
				<tr>
					<td class="subtituloDireita">E-mail</td>
					<td>'.$rs['insemail'].'</td>
				</tr>
			  </table>';		
	}
}

/**
 * 
 * @global cls_banco $db
 * @return boolean
 */
function cadastrarDirigente() {
  global $db;

  $insid = (int)$_POST['insid'];
  // -- Verificando a existência de um dirigente ativo para a mesma instituição
  $sql = <<< QUERY
SELECT COUNT(dirid)
  FROM snf.dirigentemaximo 
  WHERE insid = {$insid}
    AND dirstatus = 'A'
QUERY;

  // -- Já existe um dirigente ativo cadastrado para a instituição escolhida
  if ($db->pegaUm($sql)) { return -1; }

  $dircpf = str_replace(array('.', '-'), '', $_POST['dircpf']);
  $dirnome = str_replace("'", "''", $_POST['dirnome']);

  $sql = <<<DML
INSERT INTO snf.dirigentemaximo(insid, dirnome, dircpf, dirstatus, carid)
  VALUES({$insid}, '{$dirnome}', '{$dircpf}', 'A', 1);
DML;

  $db->executar($sql);
  return $db->commit();
}

function atualizarDirigente() {
  global $db;

  $dircpf = str_replace(array('.', '-'), '', $_POST['dircpf']);
  $dirnome = str_replace("'", "''", $_POST['dirnome']);
  $dirid = (int)$_POST['dirid'];
  
  $sql = <<<DML
UPDATE snf.dirigentemaximo
  SET dircpf = '{$dircpf}',
      dirnome = '{$dirnome}'
  WHERE dirid = {$dirid}
DML;
  $db->executar($sql);
  return $db->commit();
}

function excluirDirigente() {
  global $db;

  $dirid = (int)$_POST['dirid'];

  $sql = <<<DML
UPDATE snf.dirigentemaximo
  SET dirstatus = 'I'
  WHERE dirid = {$dirid}
DML;
  $db->executar($sql);
  return $db->commit();
}

function listarDirigentes() {
  global $db;
  $sWhere = '';
  if ($_POST) {
    if (isset($_POST['dircpf']) && $_POST['dircpf']) {
      $sWhere[] = "d.dircpf = '" . str_replace(array('.', '/', '-'), '', $_POST['dircpf']) . "'";
    }
    if (isset($_POST['dirnome']) && $_POST['dirnome']) {
      $sWhere[] = "d.dirnome ILIKE '%" . str_replace("'", "''", $_POST['dirnome']) . "%'";
    }
    if (isset($_POST['insid']) && $_POST['insid']) {
      $sWhere[] = 'i.insid = ' . (int)$_POST['insid'];
    }
    if ('' !== $sWhere) { $sWhere = ' AND ' . implode(' AND ', $sWhere); }
  }
  // -- Query com parâmetros de consulta
  $sql = <<<QUERY
SELECT '<img onclick="alterarDirigente(this, '|| d.dirid || ', ' || i.insid || ');" src="../imagens/alterar.gif" style="cursor:pointer">&nbsp;'
       || '<img onclick="excluirDirigente(this, ' || d.dirid || ');" src="../imagens/excluir.gif" style="cursor:pointer">' AS acao,
       d.dirnome,
       d.dircpf,
       i.insnome || ' - ' || coalesce(i.inssigla, '') AS insnome
  FROM snf.dirigentemaximo d
    INNER JOIN snf.instituicaoensino i
      USING(insid)
    WHERE i.insstatus = 'A'
      AND d.dirstatus = 'A' {$sWhere}
QUERY;

  $arCab = array('Açao', 'Dirigente', 'CPF', 'IES');
  $db->monta_lista($sql, $arCab, 100, 10, 'N', 'center', 'N');
}

function verificaAdesaoIES(){
	
	global $db;
	
	$sql = "select terid 
  			  from snf.termoadesao 
 			 where insid = (select insid
 			 			     from snf.dirigentemaximo 
 			 			     where dirstatus = 'A' 
 			 			       and dircpf = '".$_SESSION['usucpf']."')";	
	
	$termo = $db -> pegaUm($sql);
	
	return $termo;
}
