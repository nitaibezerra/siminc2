function processaCombosPopup()
{

selectAllOptions( document.getElementById( 'povid' ) );
selectAllOptions( document.getElementById( 'linid' ) );
selectAllOptions( document.getElementById( 'teoid' ) );
  
}


/* Função que permite somente a digitação de números. */

function somenteNumeros(e) {	
	if(window.event) {
    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
        key = e.keyCode;
    }
    else if(e.which) {
    	/* Netscape */
        key = e.which;
    }
    if(key!=8 || key < 48 || key > 57) return (((key > 47) && (key < 58)) || (key==8));
    {
    	return true;
    }
} 

function somenteLetras(e) {	
	if(window.event) {
    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
        key = e.keyCode;
    }
    else if(e.which) {
    	/* Netscape */
        key = e.which;
    }
    if(key!=8 || key < 65 || key > 122) return (((key > 65) && (key < 122)) || (key==8));
    {
    	return true;
    }
}
VertodasObras = function(){
	var form = window.document.getElementById("pesquisar");
	for(var i=0;i < form.elements.length;i++){
		var campo = form.elements[i];
		switch(campo.type){
			case "select-one":
				campo.options[0].selected = true;
			break;
			case "text":
				campo.value = "";
			break;
			case "radio":
				campo.checked = false;
			break;
			default:
				
			break;
			
		}
	}
	form.submit();
}
VerificaOrgaoSelecionado = function (orgao){
	if(orgao.value != "")
		AtualizaComboUnidadeOrgaoObra(orgao.value);
}
AtualizaComboUnidadeOrgaoObra = function(orgao){
	
	var wurl = window.location.href;
	var url_array = wurl.split("/");
	var url = url_array[0]+"//"+url_array[2]+"/";
		
	var url = url + 'obras/obras.php?modulo=inicio&acao=A&lista=1&AJAX=1';
	var parametros = "orgao="+orgao;
		
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: true,
			onComplete: function(resp) {
				var unidade = window.document.createElement("option");
						
				if(resp.responseText){
					
					Hide("loading");
					var campos = resp.responseText.split("|");
					var n = campos.length;
					window.document.getElementById("unidade").options.length = 1;
									
					if(n >1){
						for(var k=0;k < n;k++){
							var j = k+1;
							
							var valores = campos[k].split("-");
							window.document.getElementById("unidade").options[j] = new Option(valores[1],
                                                                                      valores[0],
                                                                                      false,
                                                                                      false);
						}
					}else{
						var valores = campos[0].split("-");
						window.document.getElementById("unidade").options[1] = new Option(valores[1],valores[0],false,false);	
					} 
																				
				}else{
					
					//Clear("loading");
					//Add("loading","Nenhuma Unidade encontrada para o orgão selecionado");
					//Show("loading");
					window.document.getElementById("unidade").options.length = 1;
					window.setTimeout(function(){
						Hide("loading");
					},2000)
				}
			},
			onLoading: function(){
				//Clear("loading");
				//Add("loading","Aguarde... <br/> Carregando lista de Unidades...");
				//Show("loading");
			}
		});
}  
Clear = function(id){
	window.document.getElementById(id).innerHTML = "";
}
Add =  function(id,data){
	window.document.getElementById(id).innerHTML += data;
}
Show = function(id){
	window.document.getElementById(id).style.display = "block";
}

Hide = function(id){
	window.document.getElementById(id).style.display = "none";
}

ViewBigImage = function(img,dir){
	
	var wurl = window.location.href;
	var url_array = wurl.split("/");
	var url = url_array[0]+"//"+url_array[2]+"/";
		
	var url = url + 'obras/obras.php?modulo=inicio&acao=K&AJAX=1';
	var parametros = "img="+img+"&dir="+dir;
		
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				
				if(resp.responseText){
					size = resp.responseText.split("-");
					ShowImage(img,size[0],size[1],"../" + dir);
				}else{
					alert("Imagem não encontrada.");
				}
			}
		});
		
	/*	
	var div = window.document.getElementById("big_photo");
	while(div.firstChild)
		
		div.removeChild(div.firstChild);
	
	var attributes = new Array();
	
	attributes["class"] = "image_Big_photo";
	attributes["style"] = "margin:3px;";
	attributes["src"] = "http://simec-local/obras/plugins/imgs_full/"+img;
	attributes["id"] = img;
	
	CreateElement("img",div,attributes);
	
	var attributes = new Array();
	
	attributes["class"] = "close_Big_photo";
	attributes["src"] = "http://simec-local/obras/plugins/imgs/delete.png";
	attributes["id"] = "close_Big_photo";
	
	var del = CreateElement("img",div,attributes);
	
	div.style.display = "block";
	
	del.onclick = function(){
		div.style.display = "none";
	}
	*/
}

ShowImage = function(img,w,h,dir){
	window.open('/obras/plugins/view_image.php?img=' + dir + img +'&w='+w+'&h='+h,'teste','width='+w+',height='+h)
}
UpdateListFoto = function(){
	
	var wurl = window.location.href;
	var url_array = wurl.split("/");
	var url = url_array[0]+"//"+url_array[2]+"/";
	
	var url = url + 'obras/obras.php?modulo=inicio&acao=Y&AJAX=1';
		
	var myAjax = new Ajax.Updater(
		"thumbnails",
		url,
		{
			method: 'post',
			asynchronous: false,
			onComplete: function(resp) {
				window.document.getElementById("thumbnails").innerHTML = resp.responseText;
			}
		});
	
}	
Cadastrar = function(url){
		 window.location = url;
		return; 
}

Excluir = function(url,obrid){
		if(confirm("Deseja realmente excluir esta obra ?"))
			window.location = url+'&obrid='+obrid;
}

AtualizarVistoria = function(url,supvid){
			//VerificaVistoria(url,supvid)
			window.location = url+'&supvid='+supvid;
}

ExcluirVistoria = function(url,supvid){
		if(confirm("Deseja realmente excluir esta vistoria ?"))
			VerificaVistoria(url,supvid)
}
		
VerificaVistoria = function(caminho,supervisao){
	
	var wurl = window.location.href;
	var url_array = wurl.split("/");
	var url = url_array[0]+"//"+url_array[2]+"/";
	
	var url = url + 'obras/obras.php?modulo=inicio&acao=N&AJAX=1';
	var parametros = "?supvid=" + supervisao;
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: true,
			onComplete: function(resp) {
											
				if(!(resp.responseText == '1')){
					window.location = caminho+'&supvid='+supervisao;
					
				}else{
					alert("Não é possível alterar/deletar esta vistoria, pois existe(m) outra(s) após a mesma.")
				}	
			}
		});	
}
ExcluirDocumento = function(url,arqid,aqoid){
		if(confirm("Deseja realmente excluir este documento ?"))
			window.location = url+'&aqoid='+aqoid+'&arqid='+arqid;
}
DownloadArquivo = function(arqid){
			window.location = '?modulo=inicio&acao=L'+'&arqid='+arqid;
}

Atualizar = function(url,obrid){
			window.location = url+'&obrid='+obrid;
}

AbrirPopUp = function(url,nome,param){
	window.open(url,nome,param);
}
Ordem = function(valor){
	var index = valor;
	this.index = index;
}
AtualizaFotos = function(){
	
	var wurl = window.location.href;
	var url_array = wurl.split("/");
	var url = url_array[0]+"//"+url_array[2]+"/";
	
	var inputs = window.document.getElementsByTagName("input");
	var ordem  = 0;
	var params = ""; 
	for (var k in inputs){
		var elemento = inputs[k];
		if (elemento.type == "hidden") {
			if (elemento.id != "") {
				params += "ordem[]="+ ordem +"&box[]="+elemento.name+"&foto[]="+elemento.value+"&";				
				ordem++;
				}
		}
				
	}
	var url = url + 'obras/obras.php?modulo=inicio&acao=T&AJAX=1';
	var parametros = params;
	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				UpdateListFoto();
			}
		});
}
ImageComponent = function(params){
	var a = window.open("plugins/component_foto.php"+params,"inserir_fotos","height=540,width=630");
	a.focus();
}

function abreMapa(){
	var graulatitude = window.document.getElementById("graulatitude").value;
	var minlatitude  = window.document.getElementById("minlatitude").value;
	var seglatitude  = window.document.getElementById("seglatitude").value;
	var pololatitude = window.document.getElementById("pololatitude").value;
	
	var graulongitude = window.document.getElementById("graulongitude").value;
	var minlongitude  = window.document.getElementById("minlongitude").value;
	var seglongitude  = window.document.getElementById("seglongitude").value;
	
	var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
	var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
	
	window.open( '?modulo=principal/mapa&acao=A&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude, 'blank','height=600,width=600,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no' );
}

function inserirEntidade(entid, orgid){
	if (entid){
			return windowOpen( '?modulo=principal/inserir_entidade&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&orgid=' + orgid,'blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( '?modulo=principal/inserir_entidade&acao=A&orgid=' + orgid,'blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}

function inserirEmpresa(entidempresa){
	if (entidempresa){ 
			return windowOpen( '?modulo=principal/inserir_empresa&acao=A&busca=entnumcpfcnpj&entid=' + entidempresa,'blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( '?modulo=principal/inserir_empresa&acao=A','blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}

function inserirResponsavel(entid){
	if (entid){ 
			return windowOpen( '?modulo=principal/inserir_responsavel&acao=A&busca=entnumcpfcnpj&entid=' + entid,'blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}else{
			return windowOpen( '?modulo=principal/inserir_responsavel&acao=A','blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
		}
}

function atualizaResponsavel(ent){
	var entid = ent.replace('linha_', '');
	return windowOpen( '?modulo=principal/inserir_responsavel&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&tr=' + ent,'blank','height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' )	;
}

function inserirEtapas(){
	return windowOpen('?modulo=principal/inserir_etapas&acao=A','blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function adicionarFases(){
	return windowOpen('?modulo=principal/fases_licitacao&acao=A','blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function atualizaFase(id,flcid){
	return windowOpen('?modulo=principal/fases_licitacao&acao=A&tflid=' +id+'&flcid='+flcid,'blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function preencheValor()
{
	var custoTotal = document.getElementById("obrcustocontrato").value.replace('.', '');
	    custoTotal = custoTotal.replace('.','');
	    custoTotal = Number(custoTotal.replace(',', '.'));

	var area       = document.getElementById("obrqtdconstruida").value.replace('.', '');
	    area	   = area.replace('.','');
	    area       = Number(area.replace(',', '.'));

	var custo      = document.getElementById("obrcustounitqtdconstruida");
	
	if (custoTotal != "" && area != ""){ 
		custo.value    = (new String((custoTotal / area).toFixed(2)).replace('.', ','));
		custo.value = mascaraglobal('###.###.###,##', custo.value);
	}
}

function Validacao(){

	processaCombosPopup();
	
	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;
	
	var convenio = document.formulario.covid.value;
	if (convenio == ''){
		mensagem += 'Convênio \n';
		validacao = false;
	}
	
	var obra = document.formulario.obrid.value;
	if (obra == ''){
		mensagem += 'Obra \n';
		validacao = false;
	}
	
	var povo = document.formulario.povid.value;
	if (povo == ''){
		mensagem += 'Povos Atendidos \n';
		validacao = false;
	}
	
	var lingua = document.formulario.linid.value;
	if (lingua == ''){
		mensagem += 'Línguas \n';
		validacao = false;
	}
	
	var territorio = document.formulario.teoid.value;
	if (territorio == ''){
		mensagem += 'Territórios Atendidos \n';
		validacao = false;
	}
	
	/*
	var stoid = document.formulario.stoid.value;
	if (stoid == ''){
		mensagem += 'Situação da Obra \n';
		validacao = false;
	}
	
	
	var dtinicio = document.formulario.obrdtinicio.value;
	if (dtinicio == ''){
		mensagem += 'Início Programado \n';
		validacao = false;
	}
	
	var dttermino = document.formulario.obrdttermino.value;
	if (dtinicio == ''){
		mensagem += 'Término Programado \n';
		validacao = false;
	}
	
	
	var graulatitude = document.formulario.graulatitude.value;
	if(graulatitude > 90){
		alert("O grau de latitude informado não pode ser maior que 90°");
		document.formulario.graulatitude.focus();
		return false;
	}
	
	var minlatitude = document.formulario.minlatitude.value;
	if(minlatitude > 60){
		alert("O minuto de latitude informado não pode ser maior que 60");
		document.formulario.minlatitude.focus();
		return false;
	}
	
	var seglatitude = document.formulario.seglatitude.value;
	if(seglatitude > 60){
		alert("O segundo de latitude informado não pode ser maior que 60");
		document.formulario.seglatitude.focus();
		return false;
	}
	
	var graulongitude = document.formulario.graulongitude.value;
	if(graulongitude > 90){
		alert("O grau de longitude informado não pode ser maior que 90°");
		document.formulario.graulongitude.focus();
		return false;
	}
	
	var minlongitude = document.formulario.minlongitude.value;
	if(minlatitude > 60){
		alert("O minuto de longitude informado não pode ser maior que 60");
		document.formulario.minlongitude.focus();
		return false;
	}

	var seglongitude = document.formulario.seglongitude.value;
	if(seglatitude > 60){
		alert("O segundo de longitude informado não pode ser maior que 60");
		document.formulario.seglongitude.focus();
		return false;
	}
	
	if (document.formulario.obrdtinicio.value != ""){
		if (!validaData(document.formulario.obrdtinicio)){
			alert("A data de início informada é inválida");
			document.formulario.obrdtinicio.focus();
			return false;
		}
	}
	
	if (document.formulario.obrdttermino.value != ""){
		if (!validaData(document.formulario.obrdttermino)){
			alert("A data de termino informada é inválida");
			document.formulario.obrdttermino.focus();
			return false;
		}
	}	
	
	if (document.formulario.obrdtinicio.value != "" && document.formulario.obrdttermino.value != ""){
		if (!validaDataMaior(document.formulario.obrdtinicio, document.formulario.obrdttermino)){
			alert("A data de término não deve ser maior do que a de início da obra.");
			document.formulario.obrdtinicio.focus();
			return false;
		}
	}
	*/
	
	if (!validacao){
		alert(mensagem);
	}
	
	/*
	window.document.getElementById("muncod").disabled = false;
	window.document.getElementById("endbai").disabled = false;
	window.document.getElementById("orgid").disabled = false;
	*/
	
	
	
	return validacao;
}

function validaVistoria(formu)
{
	var form        = document.getElementById(formu);
	var numelements = form.elements.length;
		
	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;

	var supvdt = document.formulario.supvdt.value;
	if (supvdt == ''){
		mensagem += 'Data da Vistoria \n';
		validacao = false;
	}

	var stoid = document.formulario.stoid.value;
	if (stoid == ''){
		mensagem += 'Situação Atual \n';
		validacao = false;
	}

	var supprojespecificacoes = document.getElementsByName("supprojespecificacoes");
	if (supprojespecificacoes.checked == false){
		mensagem += 'Projeto/Especificações \n';
		validacao = false;
	}

	var supplacaobra = document.formulario.supplacaobra;
	if (supplacaobra.checked == false){
		mensagem += 'Placa da Obra \n';
		validacao = false;
	}

	var supplacalocalterreno = document.formulario.supplacalocalterreno;
	if (supplacalocalterreno.checked == false){
		mensagem += 'Placa Indicativa do Programa/Localização do Terreno \n';
		validacao = false;
	}

	var qlbid = document.formulario.qlbid.value;
	if (qlbid == ''){
		mensagem += 'Qualidade da Execução da Obra \n';
		validacao = false;
	}

	var dcnid = document.formulario.dcnid.value;
	if (dcnid == ''){
		mensagem += 'Desempenho da Construtora \n';
		validacao = false;
	}

	if (document.formulario.supvdt.value != ""){
		if(!validaData(document.formulario.supvdt)){
			alert("A data informada é inválida");
			document.formulario.supvdt.focus();
			return false;
		}
	}

	if (!validacao){
		alert(mensagem);
	}

	return validacao;
}
validaProjetoArquitetonico = function(form){
	//['tpaid','felid','tfpid','fprdtiniciofaseprojeto','fprdtprevterminoprojeto','fprdtconclusaofaseprojeto']
	var mensagem = "Os seguintes campos devem ser preenchidos: \n\n";
	var validacao = true;
	
	
	var tpaid = form.tpaid.value;
	var felid = form.felid.value;
	var tfpid = form.tfpid.value;
	
	if(tpaid != '' || felid != '' || tfpid != '')
	{
		if (tpaid == ''){
			mensagem += 'Tipo de Projeto \n';
			validacao = false;
		}
		if (felid == ''){
			mensagem += 'Forma de Elaboração do projeto \n';
			validacao = false;
		}
		if (tfpid == ''){
			mensagem += 'Fases do Projeto \n';
			validacao = false;
		}else{
			switch(tfpid){
				case "1":
					var fprdtiniciofaseprojeto = form.fprdtiniciofaseprojeto.value;
					if (fprdtiniciofaseprojeto == ''){
						mensagem += 'Previsao de Início \n';
						validacao = false;
					}			
				break
				case "2":
					var fprdtprevterminoprojeto = form.fprdtprevterminoprojeto.value;
					if (fprdtprevterminoprojeto == ''){
						mensagem += 'Previsão de Término \n';
						validacao = false;
					}
				break
				case "3":
					var fprdtconclusaofaseprojeto = form.fprdtconclusaofaseprojeto.value;
					if (fprdtconclusaofaseprojeto == ''){
						mensagem += 'Data da conclusão \n';
						validacao = false;
					}
				break
			}
		}
		
		if(!validacao)
			alert(mensagem);
	}		
	return validacao;
}

function validaFases(){
	var mensagem = "Os seguintes campos devem ser preenchidos: \n\n";
	var validacao = true;
	
	var frpid = document.getElementById("frpid");
	if (frpid.value == ""){
		mensagem += "Tipo de Forma de Repasse de Recursos \n";
		validacao = false;
	}
	
	if (!validacao){
		alert(mensagem);
	}
	
	return validacao;
	
}

function validaInfraEstrutura(){
	
	var iexinfexistedimovel = document.formulario.iexinfexistedimovel;
	
	for (k = 0; k < iexinfexistedimovel.length; k++){	
		if (iexinfexistedimovel[k].checked == true){
			if (iexinfexistedimovel[k].value == 0){
				window.document.getElementById("iexareaconstruida").disabled = false;
				window.document.getElementById("umdid").disabled = false;
				window.document.getElementById("iexdescsumariaedificacao").disabled = false;
				window.document.getElementById("iexedificacaoreforma").disabled = false;
				window.document.getElementById("iexampliacao").disabled = false;				
			}
		}
	}			
}
function EfetuarDownload(formid, caminho) {
	var formulario = document.getElementById(formid);
	formulario.method = 'post';
	formulario.action = caminho;
	formulario.submit();
}
/*function Validacao(form,obrigatorios){
	alert("entrou");
		
	var campos_obrigatorios = obrigatorios;
	
	alert(campos_obrigatorios);
	
	var mensagem = "Os seguintes campos devem ser preenchidos: \n";
	var validacao = true;
	var qtd_elementos = form.elements.length;
	
	for(k=0; k<qtd_elementos; k++){
		
		var elemento_atual = form.elements[k];
		
		alert(campos_obrigatorios.indexOf(elemento_atual.name.toString()));
		
		alert(elemento_atual.name);
		if(campos_obrigatorios.indexOf(elemento_atual.name) != -1){
			
			switch(elemento_atual.type){
				case "text":
				case "select-one":
					if(elemento_atual.value == "" || elemento_atual.value.length == 0){
						mensagem += elemento_atual.name += '\n';
						validacao = false	
					}
				break;	
				
				case "textarea":
					if(elemento_atual.value == "" || elemento_atual.value.length == 0){
						alert(mensagem += elemento_atual.name);
						elemento_atual.focus();
						validacao = false
					}
				break;
				
			}
		}
	}
	alert(validacao);
	
	if ( !validacao ) {
		alert( mensagem );
	}
	return validacao;	
}
*/