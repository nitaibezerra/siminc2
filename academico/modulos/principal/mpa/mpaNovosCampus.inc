<?php
if($_REQUEST['reqs']=='salvarCampus'){
	salvarCampus($_POST);
	exit();
}

function verificarCampus(){
	global $db;	
	
	if($_SESSION['academico']['entid']){
		$aryWhere[] = "entidunidade = '{$_SESSION['academico']['entid']}'";
	}
	
	if($_SESSION['academico']['orgid'] == '1'){
		$funid = ACA_ID_UNIVERSIDADE ;
	}		

 	$sql = "SELECT		COUNT(nvcid)
			FROM		academico.novoscampus nvc
			LEFT JOIN 	entidade.entidade e ON e.entid = nvc.entidunidade
			LEFT JOIN 	entidade.endereco ed ON ed.entid = e.entid
 						".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '').""; 
 	
 	$campus = $db->pegaUm($sql);
 	return $campus;
}

function selecionarCampus(){
	global $db;
	
	$perfis = pegaPerfilGeral();
	
	if($_SESSION['academico']['entid']){
		$aryWhere[] = "entidunidade = '{$_SESSION['academico']['entid']}'";
	}
	
	if($_SESSION['academico']['orgid'] == '1'){
		$funid = ACA_ID_UNIVERSIDADE ;
	}	

	$alinhamento = array('center','','center','center');
	$param['totalLinhas'] = false;
	
	if(in_array(PERFIL_ADMINISTRADOR,$perfis) || $db->testa_superuser()){
		$cabecalho = array('Ação', 'Campus','Previsto','Realizado');
		$acao = "'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar\" style=\"cursor:pointer\" class=\"alterar\" id=\"alterar_'|| nvcid ||'\" onclick=\"alterarCampus('|| nvcid ||');\">
				  <input type=\"hidden\" name=\"nvcid[]\" id=\"nvcid\" value=\"'|| nvcid ||'\">' AS acao,";
	} else {
		$cabecalho = array('Campus','Previsto','Realizado');
		$acao = "";
	}	

	$sql = "SELECT 		$acao
	   					nvcdsc as campus,
			            CASE WHEN nvcprevisto IS NOT NULL THEN '<input id=\"nvcprevisto_'|| nvcid ||'\" type=\"text\" class=\"disabled\" onkeyup=\"this.value=mascaraglobal(\'####.#\',this.value);\" value=\"'|| nvcprevisto ||'\" name=\"nvcprevisto_'|| nvcid ||'\" maxlenght=\"6\" size=\"7\">' 
			            	 						      ELSE '<input id=\"nvcprevisto_'|| nvcid ||'\" type=\"text\" class=\"disabled\" onkeyup=\"this.value=mascaraglobal(\'####.#\',this.value);\" value=\"\" name=\"nvcprevisto_'|| nvcid ||'\" maxlenght=\"6\" size=\"7\">' END AS nvcprevisto,
			            CASE WHEN nvcrealizado IS NOT NULL THEN '<input id=\"nvcrealizado_'|| nvcid ||'\" type=\"text\" class=\"disabled\" onkeyup=\"this.value=mascaraglobal(\'####.#\',this.value);\" value=\"'|| nvcrealizado ||'\" name=\"nvcrealizado_'|| nvcid ||'\" maxlenght=\"6\" size=\"7\">' 
			            								   ELSE '<input id=\"nvcrealizado_'|| nvcid ||'\" type=\"text\" class=\"disabled\" onkeyup=\"this.value=mascaraglobal(\'####.#\',this.value);\" value=\"\" name=\"nvcrealizado_'|| nvcid ||'\" maxlenght=\"6\" size=\"7\">' END AS nvcrealizado 
			FROM		academico.novoscampus nvc
			LEFT JOIN 	entidade.entidade e ON e.entid = nvc.entidunidade
			LEFT JOIN 	entidade.endereco ed ON ed.entid = e.entid
						".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
			ORDER BY	nvcdsc"; 

	$db->monta_lista($sql, $cabecalho, '80', '10', 'N', '', 'N', 'formulario_campus', '', $alinhamento, '', $param);
}

function salvarCampus($post){
    global $db;	
 
    extract($post);
    
    $habilita = '';
    $perfis = pegaPerfilGeral();

    if(is_array($nvcid)){
		$sql = "";
    	
		foreach($nvcid AS $n){
	   		$nvcprevisto = $post['nvcprevisto_'.$n] ? $post['nvcprevisto_'.$n] : 'null';
	   		$nvcrealizado = $post['nvcrealizado_'.$n] ? $post['nvcrealizado_'.$n] : 'null';  		
	
			if(in_array(PERFIL_ADMINISTRADOR,$perfis) || $db->testa_superuser()){
				$habilita = "nvcprevisto = {$nvcprevisto},";
			}
			
	    	$sql .= "UPDATE 	academico.novoscampus 
	    			 SET		$habilita
	    						nvcrealizado = {$nvcrealizado}
	    			 WHERE		nvcid = {$n};";
	    }
		$db->executar($sql);
    }
    
	if($db->commit()){
		$db->sucesso('principal/monitoramentoProgramasAcoes&acao=A&aba=NovosCampus', '', 'Dados cadastrado com sucesso!', 'N', 'N');
	} else {
		$db->insucesso('Não foi possível realizar a operação.', '', 'principal/monitoramentoProgramasAcoes&acao=A&aba=NovosCampus');
	}    
}

$perfis = pegaPerfilGeral();

$habilita = 'nvcrealizado';
if(in_array(PERFIL_ADMINISTRADOR,$perfis) || $db->testa_superuser()){
	$habilita = 'nvcprevisto';
};

$campus = verificarCampus();
monta_titulo('Novos Campus', '');
?>	

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript">
	function alterarCampus(nvcid){
		if($('#alterar_'+nvcid).attr('src') == '../imagens/alterar_pb.gif'){
			$('#alterar_'+nvcid).attr('src','../imagens/alterar.gif');
			
			$('#nvcprevisto_'+nvcid).attr('title','Editar');
			$('#nvcprevisto_'+nvcid).attr('disabled',true);
			$('#nvcprevisto_'+nvcid).attr('readonly',true);
			$('#nvcprevisto_'+nvcid).removeClass('normal');
			$('#nvcprevisto_'+nvcid).addClass('disabled');

			$('#nvcrealizado_'+nvcid).attr('title','Editar');
			$('#nvcrealizado_'+nvcid).attr('disabled',true);
			$('#nvcrealizado_'+nvcid).attr('readonly',true);
			$('#nvcrealizado_'+nvcid).removeClass('normal');
			$('#nvcrealizado_'+nvcid).addClass('disabled');
		} else {
			$('#alterar_'+nvcid).attr('src','../imagens/alterar_pb.gif');

			$('#nvcprevisto_'+nvcid).attr('title','Bloquear');
			$('#nvcprevisto_'+nvcid).attr('disabled',false);
			$('#nvcprevisto_'+nvcid).attr('readonly',false);
			$('#nvcprevisto_'+nvcid).removeClass('disabled');
			$('#nvcprevisto_'+nvcid).addClass('normal');

			$('#nvcrealizado_'+nvcid).attr('title','Bloquear');
			$('#nvcrealizado_'+nvcid).attr('disabled',false);
			$('#nvcrealizado_'+nvcid).attr('readonly',false);
			$('#nvcrealizado_'+nvcid).removeClass('disabled');
			$('#nvcrealizado_'+nvcid).addClass('normal');
		} 
	}

	function salvarCampus(){
		$('#formulario_campus').append('<input id="reqs" type="hidden" name="reqs" value="salvarCampus" />');
        $('#formulario_campus').submit();
    }
</script>

<?php 
if($campus && (in_array(PERFIL_ADMINISTRADOR,$perfis) || $db->testa_superuser())){ 
	selecionarCampus(); ?>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">			
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type="button" value="Salvar" id="btnSalvar" onclick="salvarCampus();"/>
			</td>
		</tr>				
	</table>
<?php } ?>
