<script>
if(confirm('Confirma a Conclusão?')){
	window.location = "?modulo=principal/baixadebens/<?php echo $_GET['urlok']; ?>&acao=A&confirmado=S&bebid="+<?php echo $_GET['bebid']; ?>+"";
}
else{
	window.location = "?modulo=principal/baixadebens/<?php echo $_GET['urlcancel']; ?>&acao=A";
}
</script>